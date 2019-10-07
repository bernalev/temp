#include "AABBTree.h"
#include "insert_box_into_box.h"

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
	// page 305 of textbook has pseudo code for creating the tree
	BoundingBox C;

	if (num_leaves == 2) {
		this->left = objects[0];
		this->right = objects[1];

		insert_box_into_box(objects[0]->box, C);
		insert_box_into_box(objects[1]->box, C);

		this->box = C;
	}
	else {
		for (int i = 0; i < objects.size(); i++) {
			insert_box_into_box(objects[i]->box, C);
		}
		this->box = C;
		Eigen::Vector3d axis_l;
		int axis;
		double m; 

		// find the midpoint m of the bouding box along axis (largest)
		axis_l[0] = C.max_corner.x() - C.min_corner.x();
		axis_l[1] = C.max_corner.y() - C.min_corner.y();
		axis_l[2] = C.max_corner.z() - C.min_corner.z();
		if (axis_l[0] > axis_l[1] && axis_l[0] > axis_l[2]) {
			axis = 0;
			m = C.center().x();
		}
		else if (axis_l[1] > axis_l[2]) {
			axis = 1;
			m = C.center().y();
		}
		else {
			axis = 2;
			m = C.center().z();
		}

		// partition into lists surrounding m
		std::vector<std::shared_ptr<Object>> left;
		std::vector<std::shared_ptr<Object>> right;
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->box.center()[axis] < m) {
				left.push_back(objects[i]);
			}
			else
				right.push_back(objects[i]);
		}
		if (a_depth == 100) {}
		else{

		if (left.size() == 1)
			this->left = left[0];
		else 
			this->left = std::make_shared<AABBTree>(left, a_depth + 1);

		if (right.size() == 1)
			this->right = right[0];
		else
			this->right = std::make_shared<AABBTree>(right, a_depth + 1);
		}
	}
	

}
// Construct a axis-aligned bounding box tree given a list of objects. Use the
  // midpoint along the longest axis of the box containing the given objects to
  // determine the left-right split.
  //
  // Inputs:
  //   objects  list of objects to store in this AABBTree
  //   Optional inputs:
  //     depth  depth of this tree (usually set by constructor of parent as
  //       their depth+1)
  // Side effects: num_leaves is set to objects.size() and left/right pointers
  // set to subtrees or leaf Objects accordingly.