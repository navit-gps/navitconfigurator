/**
 * NavitConfigurator, makes it easy to configure Navit
 * Copyright (C) 2012 Raimar Bühmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
