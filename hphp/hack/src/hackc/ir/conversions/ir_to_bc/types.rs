// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.

use ffi::Maybe;
use ffi::Str;
use hhbc::hhas_type::Constraint;
use hhbc::hhas_type::HhasTypeInfo;
use hhvm_types_ffi::ffi::TypeConstraintFlags;
use lazy_static::lazy_static;

lazy_static! {
    static ref BUILTIN_NAME_VOID: Str<'static> = Str::new(br"HH\void");
}

fn ir2bc<'a>(ty: &ir::Type<'a>) -> HhasTypeInfo<'a> {
    use ir::Type;
    match ty {
        Type::Empty => HhasTypeInfo {
            user_type: Maybe::Just(ffi::Slice::empty()),
            type_constraint: Constraint {
                name: Maybe::Nothing,
                flags: TypeConstraintFlags::NoFlags,
            },
        },
        Type::Flags(flags, inner) => {
            let mut ty = ir2bc(inner);
            ty.type_constraint.flags = *flags;
            ty
        }
        Type::None => unreachable!(),
        Type::User(name) => HhasTypeInfo {
            user_type: Maybe::Just(*name),
            type_constraint: Constraint {
                name: Maybe::Just(*name),
                flags: TypeConstraintFlags::NoFlags,
            },
        },
        Type::UserNoConstraint(name) => HhasTypeInfo {
            user_type: Maybe::Just(*name),
            type_constraint: Constraint {
                name: Maybe::Nothing,
                flags: TypeConstraintFlags::NoFlags,
            },
        },
        Type::UserWithConstraint(name, constraint) => HhasTypeInfo {
            user_type: Maybe::Just(*name),
            type_constraint: Constraint {
                name: Maybe::Just(*constraint),
                flags: TypeConstraintFlags::NoFlags,
            },
        },
        Type::Void => HhasTypeInfo {
            user_type: Maybe::Just(*BUILTIN_NAME_VOID),
            type_constraint: Constraint {
                name: Maybe::Nothing,
                flags: TypeConstraintFlags::NoFlags,
            },
        },
    }
}

pub(crate) fn convert<'a>(ty: &ir::Type<'a>) -> Maybe<HhasTypeInfo<'a>> {
    if *ty == ir::Type::None {
        Maybe::Nothing
    } else {
        Maybe::Just(ir2bc(ty))
    }
}
