/*
    SuperCollider Qt IDE
    Copyright (c) 2012 Jakob Leben & Tim Blechmann
    http://www.audiosynth.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "main_window.hpp"
#include "post_window.hpp"
#include "util/gui_utilities.hpp"
#include "../core/main.hpp"
#include "../core/settings/manager.hpp"

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QScrollBar>
#include <QShortcut>
#include <QKeyEvent>

namespace ScIDE {

PostWindow::PostWindow(QWidget* parent):
    QPlainTextEdit(parent)
{
    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setFrameShape( QFrame::NoFrame );

    QRect availableScreenRect = qApp->desktop()->availableGeometry(this);
    mSizeHint = QSize( availableScreenRect.width() * 0.4, availableScreenRect.height() * 0.3 );

    createActions( Main::settings() );

    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(this, SIGNAL(scrollToBottomRequest()),
            this, SLOT(scrollToBottom()), Qt::QueuedConnection);

    applySettings( Main::settings() );
}

void PostWindow::createActions( Settings::Manager * settings )
{
    QAction * action;

    mActions[Copy] = action = new QAction(tr("Copy"), this);
    action->setShortcut( QKeySequence::Copy );
    action->setShortcutContext( Qt::WidgetShortcut );
    action->setEnabled( false );
    connect(action, SIGNAL(triggered()), this, SLOT(copy()));
    connect( this, SIGNAL(copyAvailable(bool)), action, SLOT(setEnabled(bool)) );
    addAction(action);

    mActions[Clear] = action = new QAction(tr("Clear"), this);
    action->setStatusTip(tr("Clear post window"));
    action->setShortcutContext(Qt::ApplicationShortcut);
    action->setShortcut(tr("Ctrl+Shift+C", "Clear Post Window"));
    settings->addAction( action, "post-clear", "Post Window" );
    connect(action, SIGNAL(triggered()), this, SLOT(clear()));
    addAction(action);

    action = new QAction(this);
    action->setSeparator(true);
    addAction(action);

    mActions[ZoomIn] = action = new QAction(tr("Enlarge Font"), this);
    action->setIconText("+");
    action->setShortcutContext( Qt::WidgetShortcut );
    action->setToolTip(tr("Enlarge font"));
    connect(action, SIGNAL(triggered()), this, SLOT(zoomIn()));
    addAction(action);

    mActions[ZoomOut] = action = new QAction(tr("Shrink Font"), this);
    action->setIconText("-");
    action->setShortcutContext( Qt::WidgetShortcut );
    action->setToolTip(tr("Shrink font"));
    connect(action, SIGNAL(triggered()), this, SLOT(zoomOut()));
    addAction(action);

    action = new QAction(this);
    action->setSeparator(true);
    addAction(action);

    mActions[LineWrap] = action = new QAction(tr("Wrap Text"), this);
    action->setCheckable(true);
    addAction(action);
    connect( action, SIGNAL(triggered(bool)), this, SLOT(setLineWrap(bool)) );

    mActions[AutoScroll] = action = new QAction(tr("Auto Scroll"), this);
    action->setToolTip(tr("Scroll to bottom on new posts"));
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(onAutoScrollTriggered(bool)));
    addAction(action);
}

void PostWindow::updateActionShortcuts( Settings::Manager * settings )
{
    settings->beginGroup("IDE/shortcuts");
    mActions[ZoomIn]->setShortcut( settings->shortcut("editor-enlarge-font") );
    mActions[ZoomOut]->setShortcut( settings->shortcut("editor-shrink-font") );
    settings->endGroup();
}

void PostWindow::applySettings(Settings::Manager * settings)
{
    int scrollback = settings->value("IDE/postWindow/scrollback").toInt();

    QFont font = settings->codeFont();

    QPalette palette;
    settings->beginGroup("IDE/editor/colors");
    if (settings->contains("text")) {
        QTextCharFormat format = settings->value("text").value<QTextCharFormat>();
        QBrush bg = format.background();
        QBrush fg = format.foreground();
        if (bg.style() != Qt::NoBrush)
            palette.setBrush(QPalette::Base, bg);
        if (fg.style() != Qt::NoBrush)
            palette.setBrush(QPalette::Text, fg);
    }
    settings->endGroup(); // colors

    bool lineWrap = settings->value("IDE/postWindow/lineWrap").toBool();

    setMaximumBlockCount(scrollback);
    setFont(font);
    setPalette(palette);
    setLineWrap( lineWrap );

    QFontMetrics metrics (font);
    QString stringOfSpaces (settings->value("IDE/editor/indentWidth").toInt(), QChar(' '));
    setTabStopWidth(metrics.width(stringOfSpaces));

    updateActionShortcuts(settings);
}

void PostWindow::storeSettings( Settings::Manager * settings )
{
    settings->setValue("IDE/postWindow/lineWrap", mActions[LineWrap]->isChecked() );
}

QString PostWindow::symbolUnderCursor()
{
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection())
        return cursor.selectedText();
    else
    {
        QString blockString = cursor.block().text();
        int position = cursor.positionInBlock();
        return wordInStringAt( position, blockString );
    }
}

void PostWindow::post(const QString &text)
{
    QScrollBar *scrollBar = verticalScrollBar();
    bool scroll = mActions[AutoScroll]->isChecked();

    QTextCursor c(document());
    c.movePosition(QTextCursor::End);
    c.insertText(text);

    if (scroll)
        emit(scrollToBottomRequest());
}

void PostWindow::scrollToBottom()
{
    verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
}

void PostWindow::onAutoScrollTriggered(bool on)
{
    if (on)
        scrollToBottom();
}

void PostWindow::zoomIn(int steps)
{
    zoomFont(steps);
}

void PostWindow::zoomOut(int steps)
{
    zoomFont(-steps);
}

void PostWindow::zoomFont(int steps)
{
    QFont currentFont = font();
    const int newSize = currentFont.pointSize() + steps;
    if (newSize <= 0)
        return;
    currentFont.setPointSize(newSize);
    setFont(currentFont);
}

static QKeySequence keySequence( QKeyEvent *event )
{
    int keys = event->modifiers();;
    switch ( event->key() ) {
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Meta:
    case Qt::Key_Alt:
        break;
    default:
        keys |= event->key();
    }

    return QKeySequence(keys);
}

bool PostWindow::event( QEvent * event )
{
    switch (event->type()) {
    case QEvent::ShortcutOverride: {
        QKeyEvent *kevent = static_cast<QKeyEvent*>(event);
        QKeySequence eventKeySequence = keySequence(kevent);
        for(int idx = 0; idx < ActionCount; ++idx) {
            if (eventKeySequence.matches( mActions[idx]->shortcut() ) == QKeySequence::ExactMatch)
            {
                mActions[idx]->trigger();
                event->accept();
                return true;
            }
        }
        break;
    }
    default:
        break;
    }

    return QPlainTextEdit::event(event);
}

void PostWindow::wheelEvent( QWheelEvent * e )
{
    if (e->modifiers() == Qt::ControlModifier) {
        if (e->delta() > 0)
            zoomIn();
        else
            zoomOut();
        return;
    }

    QPlainTextEdit::wheelEvent(e);
}

void PostWindow::focusOutEvent( QFocusEvent * event )
{
    if (event->reason() == Qt::TabFocusReason)
        MainWindow::instance()->focusCodeEditor();
    else
        QPlainTextEdit::focusOutEvent(event);
}

void PostWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();

    QPlainTextEdit::mouseDoubleClickEvent(e);
    extendSelectionForEnvVar(this, textCursor());

    cursor.endEditBlock();
}

bool PostWindow::openDocumentation()
{
    return Main::openDocumentation(symbolUnderCursor());
}

void PostWindow::openDefinition()
{
    Main::openDefinition(symbolUnderCursor(), MainWindow::instance());
}

void PostWindow::findReferences()
{
    Main::findReferences(symbolUnderCursor(), MainWindow::instance());
}

void PostWindow::setLineWrap(bool lineWrapOn)
{
    setLineWrapMode( lineWrapOn ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap );
    mActions[LineWrap]->setChecked(lineWrapOn);
    Main::settings()->setValue( "IDE/postWindow/lineWrap", lineWrapOn );
}

PostDocklet::PostDocklet(QWidget* parent):
    Docklet(tr("Post window"), parent)
{
    setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    mPostWindow = new PostWindow;
    setWidget(mPostWindow);

    toolBar()->addAction( mPostWindow->mActions[PostWindow::AutoScroll] );

    //connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(onFloatingChanged(bool)));
}

void PostDocklet::onFloatingChanged(bool floating)
{
    // HACK: After undocking when main window maximized, the dock widget can not be
    // resized anymore. Apparently it has to do something with the fact that the dock
    // widget spans from edge to edge of the screen.
    // The issue is avoided by slightly shrinking the dock widget.
    if (floating)
        dockWidget()->resize(dockWidget()->size() - QSize(1,1));
}

} // namespace ScIDE
