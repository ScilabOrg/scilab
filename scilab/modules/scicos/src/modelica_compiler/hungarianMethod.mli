module type MatrixElement =
  sig
    type t
    val zero : t
    val infinity : t
    val equal : t -> t -> bool
    val compare : t -> t -> int
    val add : t -> t -> t
    val sub : t -> t -> t
  end
module type Matrix =
  sig
    type elt
    type t
    val init : int -> elt -> (int -> int -> elt) -> t
    val iterij : (int -> int -> elt -> unit) -> t -> unit
    val row_storage_iterj : int -> (int -> elt -> unit) -> t -> unit
    val update_row_storage : int -> (int -> elt -> elt) -> t -> unit
    val column_storage_iteri : int -> (int -> elt -> unit) -> t -> unit
    val update_column_storage : int -> (int -> elt -> elt) -> t -> unit
  end
module type BipartiteGraph =
  sig
    type t
    val create : int -> t
    val link : int -> int -> t -> unit
    val ford_and_fulkerson : t -> int * (int * int option) list
  end
module type S =
  sig
    type elt
    type t
    val init : int -> (int -> int -> elt) -> t
    val perform : t -> (int * int option) list
    val print_with : (elt -> unit) -> t -> unit
  end
module Make :
  functor (E : MatrixElement) ->
    functor
      (M : sig
             type elt = E.t
             type t
             val init : int -> elt -> (int -> int -> elt) -> t
             val iterij : (int -> int -> elt -> unit) -> t -> unit
             val row_storage_iterj : int -> (int -> elt -> unit) -> t -> unit
             val update_row_storage : int -> (int -> elt -> elt) -> t -> unit
             val column_storage_iteri :
               int -> (int -> elt -> unit) -> t -> unit
             val update_column_storage :
               int -> (int -> elt -> elt) -> t -> unit
           end) ->
      functor (B : BipartiteGraph) ->
        sig
          type elt = E.t
          type t
          val init : int -> (int -> int -> elt) -> t
          val perform : t -> (int * int option) list
          val print_with : (elt -> unit) -> t -> unit
        end
