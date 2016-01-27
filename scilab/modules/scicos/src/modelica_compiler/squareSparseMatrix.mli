module type MatrixElement = sig type t val equal : t -> t -> bool end
module type S =
  sig
    type elt
    type t
    val make : int -> elt -> t
    val init : int -> elt -> (int -> int -> elt) -> t
    val size : t -> int * int
    val get : t -> int -> int -> elt
    val set : t -> int -> int -> elt -> unit
    val iter : (elt -> unit) -> t -> unit
    val iterij : (int -> int -> elt -> unit) -> t -> unit
    val row_iter : int -> (elt -> unit) -> t -> unit
    val row_iterj : int -> (int -> elt -> unit) -> t -> unit
    val row_storage_iterj : int -> (int -> elt -> unit) -> t -> unit
    val update_row_storage : int -> (int -> elt -> elt) -> t -> unit
    val column_iter : int -> (elt -> unit) -> t -> unit
    val column_iteri : int -> (int -> elt -> unit) -> t -> unit
    val column_storage_iteri : int -> (int -> elt -> unit) -> t -> unit
    val update_column_storage : int -> (int -> elt -> elt) -> t -> unit
  end
module Make :
  functor (M : MatrixElement) ->
    sig
      type elt = M.t
      type t
      val make : int -> elt -> t
      val init : int -> elt -> (int -> int -> elt) -> t
      val size : t -> int * int
      val get : t -> int -> int -> elt
      val set : t -> int -> int -> elt -> unit
      val iter : (elt -> unit) -> t -> unit
      val iterij : (int -> int -> elt -> unit) -> t -> unit
      val row_iter : int -> (elt -> unit) -> t -> unit
      val row_iterj : int -> (int -> elt -> unit) -> t -> unit
      val row_storage_iterj : int -> (int -> elt -> unit) -> t -> unit
      val update_row_storage : int -> (int -> elt -> elt) -> t -> unit
      val column_iter : int -> (elt -> unit) -> t -> unit
      val column_iteri : int -> (int -> elt -> unit) -> t -> unit
      val column_storage_iteri : int -> (int -> elt -> unit) -> t -> unit
      val update_column_storage : int -> (int -> elt -> elt) -> t -> unit
    end
