struct node {
  long head;
  long tail;
  struct node * next;
} node;

int takeaway(node *list, int head, int end ) {
  node *header = NULL;
  node *tailer = NULL;
  
  header = find_node(list, head);
  tailer = find_node(list, end);

  node_iter i = get_iter(header);

  while (i <= tail) 
    remove(list, i);
  return 0;
}

