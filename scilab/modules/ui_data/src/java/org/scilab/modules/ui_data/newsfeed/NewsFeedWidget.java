/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.gui.utils.WebBrowser;

import javax.swing.JEditorPane;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.text.html.StyleSheet;
import javax.swing.text.Document;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import java.text.SimpleDateFormat;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import java.awt.Font;

/**
 * News feed widget Displays one news at a time, controlled by
 * NewsFeedController Can display news in HTML format, uses a
 * JEditorPane/HTMLEditorKit for rendering
 */
public class NewsFeedWidget extends JEditorPane implements NewsFeedEventListener {

    private NewsFeedController newsFeed;
    private HTMLEditorKit htmlEditorKit;

    public NewsFeedWidget(NewsFeedController newsFeed) {
        this.newsFeed = newsFeed;
        setEditable(false);

        // use current look & feel font
        putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES, Boolean.TRUE);
        Font font = UIManager.getFont("Label.font");
        setFont(font);

        htmlEditorKit = new HTMLEditorKit();
        setEditorKit(htmlEditorKit);
        Document doc = htmlEditorKit.createDefaultDocument();
        setDocument(doc);

        StyleSheet styleSheet = htmlEditorKit.getStyleSheet();
        styleSheet.addRule(String.format("h1 {font-weight: bold; font-size: %d;}", font.getSize()));

        addHyperlinkListener(new HyperlinkListener() {
            public void hyperlinkUpdate(HyperlinkEvent event) {
                if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                    WebBrowser.openUrl(event.getURL(), event.getDescription());
                }
            }
        });
    }

    public void newsFeedEventReceived(NewsFeedEvent evt) {
        switch (evt.getEventType()) {
            case NewsFeedEvent.NEWS_CHANGED: {
                displayNews(newsFeed.getCurrentNews());
                break;
            }
            case NewsFeedEvent.NEWSFEED_ERROR: {
                displayError(((NewsFeedErrorEvent) evt).getErrorMessage());
                break;
            }
        }
    }

    private void displayNews(News news) {
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("EEE dd MMM yyyy HH:mm");
        String dateStr = simpleDateFormat.format(news.getDate());
        StringBuilder newsHtmlContent = new StringBuilder(
            String.format("<html><body><h1>%s - %s</h1>%s", dateStr, news.getTitle(), news.getDescription()));
        if (news.getLink() != null) {
            newsHtmlContent.append(String.format("<p><a href='%s'>%s</a></p>", news.getLink(), news.getLink()));
        }
        newsHtmlContent.append("</body></html>");
        display(newsHtmlContent.toString());
    }

    private void displayError(String errorMessage) {
        String errorHtmlContent = String.format("<html><body>%s</body></html>", errorMessage);
        display(errorHtmlContent);
    }

    private void display(String htmlContent) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    setText(htmlContent);
                }
            });
        } catch (Exception e) {
        }
    }

}
