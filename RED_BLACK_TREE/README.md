# Red-Black Tree

## 📌 Project Title

* **Red-Black Tree Implementation in C**
* Self-balancing Binary Search Tree
* Implementation of insertion, deletion, searching, rotations, and validations

---

## 🎯 Project Objective

* Implement a Red-Black Tree using C.
* Maintain the properties of a Binary Search Tree.
* Maintain the Red-Black Tree balancing properties.
* Implement insertion and deletion operations.
* Perform left and right rotations.
* Search for a particular key.
* Validate the Red-Black Tree after every operation.
* Handle duplicate keys and invalid operations.
* Understand self-balancing tree concepts.

---

## 🛠️ Technologies Used

* Programming Language: **C**
* Data Structure: **Red-Black Tree**
* Concepts:

  * Binary Search Tree
  * Dynamic Memory Allocation
  * Pointers
  * Structures
  * Recursion
  * Tree Traversal
  * Rotations
  * Self-Balancing Trees
  * Validation

---

## 📂 Project Structure

* `main.c`

  * Handles user interaction.
  * Calls insertion, deletion, search, and traversal functions.

* `insert.c`

  * Handles insertion of nodes.
  * Performs insertion balancing.
  * Performs required rotations.

* `delete.c`

  * Handles deletion of nodes.
  * Performs deletion balancing.
  * Handles double-black cases.

* `search.c`

  * Searches for a given key.

* `rotate.c`

  * Implements left rotation.
  * Implements right rotation.

* `display.c`

  * Displays the tree using traversals.

* `rbt.h`

  * Contains structure definitions.
  * Contains function declarations.

---

# 🌳 Red-Black Tree Properties

The tree must satisfy the following properties:

* Every node must be either **RED** or **BLACK**.
* The root must always be **BLACK**.
* Every NULL leaf is considered **BLACK**.
* A RED node cannot have a RED child.
* Every path from a node to its descendant NULL leaves must contain the same number of BLACK nodes.
* The tree must maintain Binary Search Tree ordering.
* Left subtree values must be less than the node value.
* Right subtree values must be greater than the node value.

---

# ➕ Insertion

## Insertion Steps

* Create a new node.
* Assign the required data.
* Initially mark the new node as **RED**.
* Insert the node according to Binary Search Tree rules.
* Set the parent, left child, and right child pointers.
* Check whether balancing is required.
* If the inserted node becomes the root:

  * Change its color to BLACK.
* Otherwise:

  * Check the color of the parent.
  * If the parent is BLACK:

    * No balancing is required.
  * If the parent is RED:

    * Red-Red violation occurs.
    * Perform insertion correction.
* Check the uncle's color.
* Perform recoloring or rotations depending on the case.
* Ensure the root is BLACK after correction.
* Validate the tree after insertion.

---

# ✅ Insertion Validations

### Before Insertion

* Check whether the tree is empty.
* Check whether the input value is valid.
* Search for the value before insertion.
* Reject duplicate values if duplicates are not supported.

### During Insertion

* Verify BST ordering.
* Verify parent-child relationships.
* Ensure the newly inserted node is initially RED.
* Check for Red-Red violation.
* Check the uncle's color.
* Perform appropriate rotation/recoloring.

### After Insertion

* Root must be BLACK.
* No RED node should have a RED child.
* BST ordering must be maintained.
* Parent pointers must be correct.
* Left and right child pointers must be correct.
* Black height must be equal on all paths.
* Tree must satisfy all Red-Black Tree properties.

---

# 🔄 Insertion Cases

## Case 1: New Node is Root

* Insert the node.
* Color the node BLACK.
* No rotation required.
* Validate root color.

## Case 2: Parent is BLACK

* No Red-Red violation.
* No rotation required.
* No recoloring required.
* Tree remains valid.

## Case 3: Parent and Uncle are RED

* Recolor parent to BLACK.
* Recolor uncle to BLACK.
* Recolor grandparent to RED.
* Continue checking from grandparent.
* Ensure root becomes BLACK.

## Case 4: Parent is RED and Uncle is BLACK

* Identify whether the node is:

  * Left-Left
  * Left-Right
  * Right-Right
  * Right-Left
* Perform the required rotation.
* Recolor nodes.
* Validate the resulting subtree.

---

# ↪️ Rotations

## Left Rotation

* Verify that the node has a right child.
* Store the right child.
* Move the right child's left subtree.
* Update parent pointers.
* Make the right child the new parent.
* Update the root if required.
* Verify BST ordering after rotation.

## Right Rotation

* Verify that the node has a left child.
* Store the left child.
* Move the left child's right subtree.
* Update parent pointers.
* Make the left child the new parent.
* Update the root if required.
* Verify BST ordering after rotation.

---

# ✅ Rotation Validations

* Rotation must not lose any node.
* Parent pointers must be updated correctly.
* Child pointers must be updated correctly.
* Root pointer must be updated when rotation occurs at the root.
* BST ordering must remain valid.
* No circular links should be created.
* Red-Black properties must be restored after balancing.

---

# ❌ Deletion

## Deletion Steps

* Search for the required node.
* Check whether the node exists.
* If the node does not exist:

  * Display an appropriate message.
  * Do not modify the tree.
* If the node exists:

  * Identify the deletion case.
* Cases:

  * Node is a leaf.
  * Node has one child.
  * Node has two children.
* For a node with two children:

  * Find the inorder successor or predecessor.
  * Replace the required value.
  * Delete the successor/predecessor node.
* Check the color of the node being removed.
* If the deleted node is RED:

  * No major balancing is required.
* If the deleted node is BLACK:

  * Check the replacement child.
  * Handle the double-black condition.
* Perform recoloring and rotations as required.
* Ensure the root becomes BLACK.
* Validate the tree after deletion.

---

# ✅ Deletion Validations

### Before Deletion

* Check whether the tree is empty.
* Search for the requested value.
* If value is not found:

  * Report that the value does not exist.
  * Do not modify the tree.

### During Deletion

* Verify BST structure.
* Identify the node's number of children.
* Check the node color.
* Check the replacement node.
* Maintain parent pointers.
* Handle double-black conditions when required.

### After Deletion

* Root must be BLACK.
* No RED node can have a RED child.
* BST ordering must be maintained.
* Parent pointers must be correct.
* All paths must have equal black height.
* No node should be lost accidentally.
* No invalid child or parent links should remain.
* Red-Black Tree properties must be restored.

---

# ⚫ Double-Black Handling

* Identify the double-black node.
* Check whether it is the root.
* Check the sibling.
* Check the sibling's color.
* Check the colors of the sibling's children.
* Apply the appropriate correction case.

## Double-Black Cases

* Sibling is RED.
* Sibling is BLACK with two BLACK children.
* Sibling is BLACK with a RED near child.
* Sibling is BLACK with a RED far child.
* Perform required recoloring.
* Perform required rotations.
* Continue correction if double-black moves upward.
* Ensure the root is BLACK after correction.

---

# 🔍 Search

## Search Steps

* Start from the root.
* Compare the required value with the current node.
* If values are equal:

  * Node is found.
* If the value is smaller:

  * Move to the left subtree.
* If the value is greater:

  * Move to the right subtree.
* Continue until the value is found or NULL is reached.

---

# ✅ Search Validations

* Check whether the tree is empty.
* Verify the search key.
* Follow BST ordering.
* Do not modify the tree during search.
* Return successful result if the node exists.
* Return unsuccessful result if the node does not exist.

---

# 🌲 Traversals

## Inorder Traversal

* Visit left subtree.
* Visit root.
* Visit right subtree.
* Output should be in sorted order.
* Validate BST ordering using inorder traversal.

## Preorder Traversal

* Visit root.
* Visit left subtree.
* Visit right subtree.
* Useful for checking tree structure.

## Postorder Traversal

* Visit left subtree.
* Visit right subtree.
* Visit root.
* Useful for deletion and memory cleanup.

---

# ✅ Traversal Validations

* Every existing node should be visited.
* No node should be visited more than once.
* Inorder traversal must produce sorted values.
* Traversal must not modify the tree.
* NULL pointers must be handled safely.

---

# 🧪 Red-Black Tree Validation Function

The validation function checks:

* Root color.
* BST property.
* Red-Red violation.
* Black height.
* Parent-child relationships.
* Valid node colors.
* Tree structure.

---

# ✅ Complete Tree Validation

After every **Insertion**:

* Check root color.
* Check BST property.
* Check Red-Red violation.
* Check black height.
* Check parent pointers.

After every **Deletion**:

* Check root color.
* Check BST property.
* Check Red-Red violation.
* Check black height.
* Check parent pointers.
* Check double-black correction.

After every **Rotation**:

* Check parent pointers.
* Check child pointers.
* Check root pointer.
* Check BST ordering.
* Check Red-Black properties.

---

# ⚠️ Error Handling

* Empty tree insertion:

  * Allow insertion of the first node.
* Duplicate insertion:

  * Reject duplicate key.
* Search in empty tree:

  * Display tree-empty message.
* Search for unavailable value:

  * Display value-not-found message.
* Deletion from empty tree:

  * Display tree-empty message.
* Deletion of unavailable value:

  * Display value-not-found message.
* Invalid menu choice:

  * Display an error message.
* Memory allocation failure:

  * Handle `malloc()` failure safely.

---

# 💾 Memory Management

* Allocate memory dynamically for every new node.
* Check whether `malloc()` returns NULL.
* Free deleted nodes.
* Avoid memory leaks.
* Avoid dangling pointers.
* Do not access freed memory.
* Maintain valid parent and child pointers.

---

# 🖥️ Menu Operations

The program can provide the following options:

1. Insert Node
2. Delete Node
3. Search Node
4. Display Inorder
5. Display Preorder
6. Display Postorder
7. Validate Red-Black Tree
8. Exit

---

# 📋 Sample Operations

* Insert:

  * `10`
  * `20`
  * `30`
  * `15`
  * `25`

* Search:

  * Search for existing value.
  * Search for non-existing value.

* Delete:

  * Delete a leaf node.
  * Delete a node with one child.
  * Delete a node with two children.
  * Delete the root.
  * Delete a non-existing value.

* Validate:

  * Run validation after every operation.
  * Confirm that all Red-Black properties are maintained.

---

# 📈 Advantages

* Maintains balanced tree structure.
* Provides efficient searching.
* Provides efficient insertion.
* Provides efficient deletion.
* Prevents the tree from becoming highly skewed.
* Guarantees logarithmic height.
* Useful for implementing ordered data structures.

---

# ⏱️ Time Complexity

| Operation      | Average/Worst Case |
| -------------- | -----------------: |
| Search         |           O(log n) |
| Insertion      |           O(log n) |
| Deletion       |           O(log n) |
| Left Rotation  |               O(1) |
| Right Rotation |               O(1) |
| Traversal      |               O(n) |
| Validation     |               O(n) |

---

# 🚀 Future Enhancements

* Implement iterative insertion and deletion.
* Add graphical tree visualization.
* Add file-based input/output.
* Add support for duplicate keys.
* Implement generic Red-Black Tree using C++ templates.
* Add automated test cases.
* Add detailed debugging information.
* Add performance testing for large datasets.

---

# 🎓 Learning Outcomes

* Understand self-balancing Binary Search Trees.
* Understand Red-Black Tree properties.
* Implement tree rotations.
* Understand insertion balancing.
* Understand deletion balancing.
* Understand double-black conditions.
* Implement recursive tree traversal.
* Use dynamic memory allocation.
* Implement tree validation.
* Debug complex pointer-based data structures.

---

# 👨‍💻 Author

* **Name:** Manju Thiru
* **Project:** Red-Black Tree
* **Language:** C
* **Domain:** Data Structures

---

# 📄 License

* This project is created for educational purposes.
* The implementation can be modified and extended for learning and practice.
