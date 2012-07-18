/*
 * BoxGraphicsItemChangedListener.h
 *
 *  Created on: 23.06.2012
 *      Author: remo
 */

#ifndef BOXGRAPHICSITEMLISTENER_H_
#define BOXGRAPHICSITEMLISTENER_H_

class BoxGraphicsItem; // forward declaration needed for BoxGraphicsItem

/**
 * Interface class for events send from a BoxGraphicsItem to the DialogMapView.
 */
class BoxGraphicsItemListener {
public:
	/**
	 * Virtual destructor is needed for destroying all subclass correctly.
	 */
	virtual ~BoxGraphicsItemListener() {};
	/**
	 * A box item is selected-
	 */
	virtual void boxItemSelected(const BoxGraphicsItem& boxItem) = 0;
//	/**
//	 * The horizontal alignment of a box item has been changed.
//	 */
//	virtual void boxItemHorizontalAlignChanged(const BoxGraphicsItem& item) = 0;
//	/**
//	 * The vertical alignment of a box item has been changed.
//	 */
//	virtual void boxItemVerticalAlignChanged(const BoxGraphicsItem& item) = 0;
	/**
	 * A box item is moving to an other position.
	 */
	virtual void boxItemMoving(const BoxGraphicsItem& boxItem) = 0;
	/**
	 * The size of a box item is changing.
	 */
	virtual void boxItemResizing(const BoxGraphicsItem& boxItem) = 0;
	/**
	 * A box item has finished moving.
	 */
	virtual void boxItemMoveFinished(BoxGraphicsItem& boxItem) = 0;
	/**
	 * The size of a box item is changing.
	 */
	virtual void boxItemResizeFinished(BoxGraphicsItem& boxItem) = 0;
};

#endif /* BOXGRAPHICSITEMLISTENER_H_ */
