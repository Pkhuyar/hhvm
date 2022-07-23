(*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the "hack" directory of this source tree.
 *
 *)

open Shape_analysis_types

let parse_mode = function
  | "flag" -> Some FlagTargets
  | "dump" -> Some DumpConstraints
  | "simplify" -> Some SimplifyConstraints
  | "codemod" -> Some Codemod
  | "solve" -> Some SolveConstraints
  | _ -> None

let mk ~mode ~verbosity = { mode; verbosity }
