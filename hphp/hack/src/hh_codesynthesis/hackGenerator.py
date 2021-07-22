# pyre-strict
# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the "hack" directory of this source tree.
#
# A family of generators to create Hack constructs like class, function, interface, etc
#
# We are assuming all classes/interfaces/functions are properly defined before
# creating any implements/extends dependency. This is guaranteed by preceding
# step. In our case, it's the logic rule checked all constraints for us,
# therefore, the reasoning result from Clingo can be converted to code
# straightforward.
#
# _HackClassGenerator maintains each class definition.
# _HackInterfaceGenerator maintains each interface definition.
# _HackFunctionGenerator maintains each function definition.
# HackGenerator extends CodeGenerator combines all _Hack*Generator to
# emit Hack code on clingo output.
from typing import Set, Dict, Any, Tuple

import clingo
from hphp.hack.src.hh_codesynthesis.codeGenerator import CodeGenerator


class _HackBaseGenerator(object):
    """
    _HackBaseGenerator for the shared part of _HackInterfaceGenerator and
    _HackClassGenerator. In this case is the body of each class or interface
    definition. We are extending this to support method declaration/definition.
    """

    def __init__(self) -> None:
        super(_HackBaseGenerator, self).__init__()
        self.name = "Base"
        # A set of methods in this class/interface.
        self.methods: Set[str] = set()
        # A set of parameters invoked in dummy method.
        self.parameter_set: Set[str] = set()
        # A set of functions to invoke in dummy method.
        self.invoke_funcs_set: Set["_HackFunctionGenerator"] = set()

    def add_method(self, method_name: str) -> None:
        self.methods.add(method_name)

    def add_parameter(self, parameter_type: str) -> None:
        self.parameter_set.add(parameter_type)

    def _print_dummy_method_body(self) -> str:
        return ";"

    def _print_dummy_method(self) -> str:
        parameter_list = ", ".join(
            (map(lambda x: f"{x} ${x}_obj", sorted(self.parameter_set)))
        )
        if parameter_list == "" and len(self.invoke_funcs_set) == 0:
            return ""
        dummy_name = f"dummy_{self.name}_method"

        # We are defining a unique dummy_method among all the methods defined by
        # the user. If there is a naming conflict, simply extending it with "_".
        while dummy_name in self.methods:
            dummy_name += "_"
        return f"\npublic function {dummy_name}({parameter_list}): void{self._print_dummy_method_body()}\n"

    def _print_method_body(self) -> str:
        return ";"

    def _print_method(self, method_name: str, static_keyword: str = " ") -> str:
        return f"\npublic{static_keyword}function {method_name}(): void{self._print_method_body()}\n"

    def _print_methods(self) -> str:
        return "".join(list(map(self._print_method, sorted(self.methods))))

    def _print_static_methods(self) -> str:
        return ""

    def _print_body(self) -> str:
        return (
            "{"
            + self._print_static_methods()
            + self._print_dummy_method()
            + self._print_methods()
            + "}"
        )


class _HackInterfaceGenerator(_HackBaseGenerator):
    """A generator to emit Hack Interface definition."""

    def __init__(self, name: str, **kwargs: Dict[str, Any]) -> None:
        super(_HackInterfaceGenerator, self).__init__()
        self.name = name
        # A set of extends relationship in this interface.
        self.extends: Set[str] = set()

    def add_extend(self, extend_from: str) -> None:
        self.extends.add(extend_from)

    def _print_extends(self) -> str:
        if len(self.extends) == 0:
            return ""
        return "extends {}".format(",".join(sorted(self.extends)))

    def __str__(self) -> str:
        return f"interface {self.name} {self._print_extends()} {self._print_body()}"


class _HackClassGenerator(_HackBaseGenerator):
    """A generator to emit Hack Class definition."""

    def __init__(self, name: str, **kwargs: Dict[str, Any]) -> None:
        super(_HackClassGenerator, self).__init__()
        self.name = name
        # Extend relationship could only be one parent class.
        self.extend: str = ""
        # A set of implements relationship in this class.
        self.implements: Set[str] = set()
        # A set of static methods in this class.
        self.static_methods: Set[str] = set()
        # A set of methods to invoke in dummy method.
        self.invoke_set: Set[Tuple[str, str]] = set()
        # A set of static methods to invoke in dummy method.
        self.invoke_static_set: Set[Tuple[str, str]] = set()

    def set_extend(self, extend_from: str) -> None:
        self.extend = extend_from

    def add_implement(self, implement: str) -> None:
        self.implements.add(implement)

    def add_static_method(self, method_name: str) -> None:
        self.static_methods.add(method_name)

    def add_invoke(self, object_type: str, method_name: str) -> None:
        if object_type in self.parameter_set:
            self.invoke_set.add((object_type, method_name))

    def add_invoke_static_method(self, class_name: str, method_name: str) -> None:
        self.invoke_static_set.add((class_name, method_name))

    def add_invoke_function(self, fn_obj: "_HackFunctionGenerator") -> None:
        self.invoke_funcs_set.add(fn_obj)

    def _print_extend(self) -> str:
        if self.extend == "":
            return ""
        return "extends {}".format(self.extend)

    def _print_implements(self) -> str:
        if len(self.implements) == 0:
            return ""
        return "implements {}".format(",".join(sorted(self.implements)))

    def _print_static_methods(self) -> str:
        return "".join(
            [self._print_method(x, " static ") for x in sorted(self.static_methods)]
        )

    def _print_dummy_method_body(self) -> str:
        return (
            "{"
            + "".join([f"\n${x[0]}_obj->{x[1]}();\n" for x in sorted(self.invoke_set)])
            + "".join(
                [f"\n{x[0]}::{x[1]}();\n" for x in sorted(self.invoke_static_set)]
            )
            + "".join(
                [
                    f"\n{x._print_callee()}\n"
                    for x in sorted(self.invoke_funcs_set, key=lambda x: x.name)
                ]
            )
            + "}"
        )

    def _print_method_body(self) -> str:
        return "{}"

    def __str__(self) -> str:
        return (
            f"class {self.name} {self._print_extend()} "
            + f"{self._print_implements()} {self._print_body()}"
        )


class _HackFunctionGenerator:
    """A generator to emit Hack Function definition."""

    def __init__(self, name: str, **kwargs: Dict[str, Any]) -> None:
        self.name = name
        # A set of static methods to invoke in the function.
        self.invoke_static_set: Set[Tuple[str, str]] = set()
        # A set of functions to invoke in the function.
        self.invoke_funcs_set: Set["_HackFunctionGenerator"] = set()

    def add_invoke_static_method(self, class_name: str, method_name: str) -> None:
        self.invoke_static_set.add((class_name, method_name))

    def add_invoke_function(self, fn_obj: "_HackFunctionGenerator") -> None:
        self.invoke_funcs_set.add(fn_obj)

    def _print_body(self) -> str:
        return (
            "{"
            + "".join(
                [f"\n{x[0]}::{x[1]}();\n" for x in sorted(self.invoke_static_set)]
            )
            + "".join(
                [
                    f"\n{x._print_callee()}\n"
                    for x in sorted(self.invoke_funcs_set, key=lambda x: x.name)
                ]
            )
            + "}"
        )

    def _print_callee(self) -> str:
        return f"{self.name}();"

    def __str__(self) -> str:
        return f"function {self.name}(): void {self._print_body()}"


class HackCodeGenerator(CodeGenerator):
    """A wrapper generator encapsulates each _Hack*Generator to emit Hack Code"""

    def __init__(self) -> None:
        super(HackCodeGenerator, self).__init__()
        self.class_objs: Dict[str, _HackClassGenerator] = {}
        self.interface_objs: Dict[str, _HackInterfaceGenerator] = {}
        self.function_objs: Dict[str, _HackFunctionGenerator] = {}

    def _add_class(self, name: str) -> None:
        self.class_objs[name] = _HackClassGenerator(name)

    def _add_interface(self, name: str) -> None:
        self.interface_objs[name] = _HackInterfaceGenerator(name)

    def _add_function(self, name: str) -> None:
        self.function_objs[name] = _HackFunctionGenerator(name)

    def _add_extend(self, name: str, extend: str) -> None:
        if name in self.class_objs:
            self.class_objs[name].set_extend(extend)
        if name in self.interface_objs:
            self.interface_objs[name].add_extend(extend)

    def _add_implement(self, name: str, implement: str) -> None:
        if name in self.class_objs:
            self.class_objs[name].add_implement(implement)

    def _add_method(self, name: str, method_name: str) -> None:
        if name in self.class_objs:
            self.class_objs[name].add_method(method_name)
        if name in self.interface_objs:
            self.interface_objs[name].add_method(method_name)

    def _add_static_method(self, name: str, method_name: str) -> None:
        if name in self.class_objs:
            self.class_objs[name].add_static_method(method_name)

    def _add_to_parameter_set(self, name: str, parameter_type: str) -> None:
        if name in self.class_objs:
            self.class_objs[name].add_parameter(parameter_type)
        elif name in self.interface_objs:
            self.interface_objs[name].add_parameter(parameter_type)

    def _add_invoke_function(self, name: str, function_name: str) -> None:
        # The function didn't purely pass as a name, we are passing a reference
        # to the function object. The reason is the function is going to have
        # parameter inside, so keep a reference to the function object make
        # code synthesis easier later.
        if function_name not in self.function_objs:
            return

        if name in self.class_objs:
            self.class_objs[name].add_invoke_function(self.function_objs[function_name])
        if name in self.function_objs:
            self.function_objs[name].add_invoke_function(
                self.function_objs[function_name]
            )

    def _add_invoke(self, name: str, object_type: str, method_name: str) -> None:
        if name in self.class_objs:
            self.class_objs[name].add_invoke(object_type, method_name)

    def _add_invoke_static_method(
        self, name: str, class_name: str, method_name: str
    ) -> None:
        if name in self.class_objs:
            self.class_objs[name].add_invoke_static_method(class_name, method_name)
        if name in self.function_objs:
            self.function_objs[name].add_invoke_static_method(class_name, method_name)

    def __str__(self) -> str:
        return (
            "<?hh\n"
            + "\n".join(str(x) for x in self.class_objs.values())
            + "\n"
            + "\n".join(str(x) for x in self.interface_objs.values())
            + "\n"
            + "".join([str(x) + "\n" for x in self.function_objs.values()])
        )

    def on_model(self, m: clingo.Model) -> None:
        # Separate into 'class(?)', 'interface(?)', 'funcs(?)',
        # 'implements(?, ?)', 'extends(?, ?)', 'add_method(?, ?)',
        # 'add_static_method(?, ?)', 'has_method_with_parameter(?, ?)'
        # 'invokes_function(?, ?)'
        # 'invokes_in_method(?, ?, ?)', 'invokes_static_method(?, ?, ?)'
        predicates = m.symbols(atoms=True)
        node_func = {
            "class": self._add_class,
            "interface": self._add_interface,
            "funcs": self._add_function,
        }
        edge_func = {
            "extends": self._add_extend,
            "implements": self._add_implement,
            "add_method": self._add_method,
            "add_static_method": self._add_static_method,
            "has_method_with_parameter": self._add_to_parameter_set,
            "invokes_function": self._add_invoke_function,
        }
        trip_func = {
            "invokes_in_method": self._add_invoke,
            "invokes_static_method": self._add_invoke_static_method,
        }
        # Three passes,
        #   First pass creates individual nodes like class, interface, function.
        for predicate in predicates:
            if predicate.name in node_func:
                node_func[predicate.name](predicate.arguments[0].string)

        #   Second pass creates edge between two nodes.
        for predicate in predicates:
            if predicate.name in edge_func:
                edge_func[predicate.name](
                    predicate.arguments[0].string,
                    predicate.arguments[1].string,
                )
        #   Third pass creates relationships between three nodes.
        for predicate in predicates:
            if predicate.name in trip_func:
                trip_func[predicate.name](
                    predicate.arguments[0].string,
                    predicate.arguments[1].string,
                    predicate.arguments[2].string,
                )