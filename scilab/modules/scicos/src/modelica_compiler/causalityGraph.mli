type t = Graph of bool ref * nodes
and nodes = node array
and node = {
  contents : int;
  mutable index : int;
  mutable not_visited : bool;
  mutable root : node;
  mutable in_component : bool;
  mutable next_nodes : node list;
}
val min : node -> node -> node
val gt : node -> node -> bool
val empty : 'a list ref -> bool
val push : 'a -> 'a list ref -> unit
val pop : 'a list ref -> 'a
val top : 'a list ref -> 'a
val visit1 : node array -> int list list
val erase_marks : node array -> unit
val create : int -> t
val connect : int -> int -> t -> unit
val strongly_connected_components : t -> int list list
val print_with : (int -> 'a) -> t -> unit
