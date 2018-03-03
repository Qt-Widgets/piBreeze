/*
	*
	* PBButton.hpp - The PBButton Class, a simple replacement for QPushButton and QToolButton
	*
*/

#pragma once

#include "Global.hpp"
#include "PBGuiWidgets.hpp"

class PBButton : public QPushButton {
	Q_OBJECT

	Q_ENUMS(Segment)

	public:
		enum Segment {
			FirstSegment,
			MiddleSegment,
			LastSegment,
			SingleButton
		};

		/* A button */
		PBButton( QWidget *parent = NULL );

		/* ToolButton */
		PBButton( QIcon, QWidget *parent = NULL  );

		/* PushButton */
		PBButton( QString, QWidget *parent = NULL  );

		/* PushButton */
		PBButton( QIcon, QString, QWidget *parent = NULL  );

		/* Get/Set icon */
		QIcon icon();
		void setIcon( QIcon );

		/* Get/Set text */
		QString text();
		void setText( QString );

		/* Get/Set icon size */
		QSize iconSize();
		void setIconSize( QSize );

		/* Button Size */
		QSize size();

		/* Size hint */
		QSize sizeHint() const;

		/* ObjectName */
		void setObjectName( QString );

		/* Segment in a SegmentButton */
		int segment();
		void setSegment( PBButton::Segment );

		/* Highly customized stylesheet */
		void setButtonStyle();

	private:
		void initPrivateVariables();

		/* Icon */
		QIcon mIcon;

		/* Text */
		QString mText;

		/* Icon Size */
		QSize mIconSize;

		/* Button Size */
		int mHeight, mWidth;

		/* Button Segment */
		PBButton::Segment mSegment;

		/* Button StyleSheet info */
		QColor mButtonColor;
		QStringList radius;
		QString leftBorderSize;
};
