// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the "hack" directory of this source tree.
//
// @generated SignedSource<<c72e9655e1dcc4d6c6704eff2f29ee2b>>
//
// To regenerate this file, run:
//   hphp/hack/src/oxidized_regen.sh

use arena_trait::TrivialDrop;
use eq_modulo_pos::EqModuloPos;
use eq_modulo_pos::EqModuloPosAndReason;
use no_pos_hash::NoPosHash;
use ocamlrep_derive::FromOcamlRep;
use ocamlrep_derive::FromOcamlRepIn;
use ocamlrep_derive::ToOcamlRep;
use serde::Deserialize;
use serde::Serialize;

#[allow(unused_imports)]
use crate::*;

pub use crate::shape_map;

pub use pos::Pos;

pub type Id_ = String;

#[derive(
    Clone,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C)]
pub struct Id(pub Pos, pub Id_);

pub type Pstring = (Pos, String);

pub type ByteString = String;

pub type PositionedByteString = (Pos, bstr::BString);

#[derive(
    Clone,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C, u8)]
pub enum ShapeFieldName {
    SFlitInt(Pstring),
    SFlitStr(PositionedByteString),
    SFclassConst(Id, Pstring),
}

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum Variance {
    Covariant,
    Contravariant,
    Invariant,
}
impl TrivialDrop for Variance {}
arena_deserializer::impl_deserialize_in_arena!(Variance);

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum ConstraintKind {
    ConstraintAs,
    ConstraintEq,
    ConstraintSuper,
}
impl TrivialDrop for ConstraintKind {}
arena_deserializer::impl_deserialize_in_arena!(ConstraintKind);

pub type Reified = bool;

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum Abstraction {
    Concrete,
    Abstract,
}
impl TrivialDrop for Abstraction {}
arena_deserializer::impl_deserialize_in_arena!(Abstraction);

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C, u8)]
pub enum ClassishKind {
    /// Kind for `class` and `abstract class`
    Cclass(Abstraction),
    /// Kind for `interface`
    Cinterface,
    /// Kind for `trait`
    Ctrait,
    /// Kind for `enum`
    Cenum,
    /// Kind for `enum class` and `abstract enum class`.
    /// See https://docs.hhvm.com/hack/built-in-types/enum-class
    CenumClass(Abstraction),
}

#[derive(
    Clone,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C, u8)]
pub enum ParamKind {
    /// Contains the position for an entire `inout` annotated expression, e.g.:
    ///
    ///   foo(inout $bar);
    ///       ^^^^^^^^^^
    Pinout(Pos),
    Pnormal,
}

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum ReadonlyKind {
    Readonly,
}
impl TrivialDrop for ReadonlyKind {}
arena_deserializer::impl_deserialize_in_arena!(ReadonlyKind);

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum OgNullFlavor {
    OGNullthrows,
    OGNullsafe,
}
impl TrivialDrop for OgNullFlavor {}
arena_deserializer::impl_deserialize_in_arena!(OgNullFlavor);

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum PropOrMethod {
    IsProp,
    IsMethod,
}
impl TrivialDrop for PropOrMethod {}
arena_deserializer::impl_deserialize_in_arena!(PropOrMethod);

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum FunKind {
    FSync,
    FAsync,
    FGenerator,
    FAsyncGenerator,
}
impl TrivialDrop for FunKind {}
arena_deserializer::impl_deserialize_in_arena!(FunKind);

#[derive(
    Clone,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C, u8)]
pub enum Bop {
    /// Addition: x + y
    Plus,
    /// Subtraction: x - y
    Minus,
    /// Multiplication: x * y
    Star,
    /// Division: x / y
    Slash,
    /// Value/coercing equality: x == y
    Eqeq,
    /// Same-type-and-value equality: x === y
    Eqeqeq,
    /// Exponent: x ** y
    Starstar,
    /// Value inquality: x != y
    Diff,
    /// Not-same-type-and-value-equality: x !== y
    Diff2,
    /// Logical AND: x && y
    Ampamp,
    /// Logical OR: x || y
    Barbar,
    /// Less than: x < y
    Lt,
    /// Less than or equal to: x <= y
    Lte,
    /// Greater than: x > y
    Gt,
    /// Greater than or equal to: x >= y
    Gte,
    /// String concatenation: x . y
    Dot,
    /// Bitwise AND: x & y
    Amp,
    /// Bitwise OR: x | y
    Bar,
    /// Bitwise left shift: x << y
    Ltlt,
    /// Bitwise right shift: x >> y
    Gtgt,
    /// Modulo: x % y
    Percent,
    /// Bitwise XOR: x ^ y
    Xor,
    /// Spaceship operator: x <=> y
    Cmp,
    /// Coalesce: x ?? y
    QuestionQuestion,
    /// =, +=, -=, ...
    Eq(Option<Box<Bop>>),
}

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum Uop {
    /// Bitwise negation: ~x
    Utild,
    /// Logical not: !b
    Unot,
    /// Unary plus: +x
    Uplus,
    /// Unary minus: -x
    Uminus,
    /// Unary increment: ++i
    Uincr,
    /// Unary decrement: --i
    Udecr,
    /// Unary postfix increment: i++
    Upincr,
    /// Unary postfix decrement: i--
    Updecr,
    /// Error control/Silence (ignore) expections: @e
    Usilence,
}
impl TrivialDrop for Uop {}
arena_deserializer::impl_deserialize_in_arena!(Uop);

#[derive(
    Clone,
    Copy,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    FromOcamlRepIn,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(u8)]
pub enum Visibility {
    Private,
    Public,
    Protected,
    Internal,
}
impl TrivialDrop for Visibility {}
arena_deserializer::impl_deserialize_in_arena!(Visibility);

/// Literal values that can occur in XHP enum properties.
///
/// class :my-xhp-class {
///   attribute enum {'big', 'small'} my-prop;
/// }
#[derive(
    Clone,
    Debug,
    Deserialize,
    Eq,
    EqModuloPos,
    EqModuloPosAndReason,
    FromOcamlRep,
    Hash,
    NoPosHash,
    Ord,
    PartialEq,
    PartialOrd,
    Serialize,
    ToOcamlRep
)]
#[repr(C, u8)]
pub enum XhpEnumValue {
    XEVInt(isize),
    XEVString(String),
}
