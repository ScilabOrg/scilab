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

import org.scilab.modules.ui_data.utils.UiDataMessages;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Calendar;

/**
 * News feed controller In a single thread: updates news periodically & manages
 * news rolling Communicates through NewsFeedEvent event class URL and other
 * settings in modules/etc/ui_data/newsfeed.xml
 */
public class NewsFeedController extends Thread {

    private List<News> news;
    private Iterator<News> newsIterator = null;
    private News currentNews;

    private int newsChangeTimeInterval = 30 * 1000; // by default change news
    // every 15 seconds

    private int feedUpdateTimeInterval = 4 * 60 * 60 * 1000; // by default
    // update feed
    // every 4 hours
    private Calendar nextFeedUpdateTime;
    private NewsFetcher newsFetcher;

    private ArrayList<NewsFeedEventListener> listeners;

    public NewsFeedController() {
        newsFetcher = new NewsFetcher();
        news = new ArrayList<News>();
        nextFeedUpdateTime = Calendar.getInstance();
        listeners = new ArrayList<NewsFeedEventListener>();
    }

    public void run() {
        currentNews = null;
        while (!isInterrupted()) {
            try {
                updateNewsFeed();
                if (isOK()) {
                    changeNews();
                }
                Thread.sleep(newsChangeTimeInterval);
            } catch (InterruptedException ex) {
                Thread.currentThread().interrupt();
            }
        }
    }

    public News getCurrentNews() {
        return currentNews;
    }

    public void addNewsFeedEventListener(NewsFeedEventListener listener) {
        listeners.add(listener);
    }

    public void removeNewsFeedEventListener(NewsFeedEventListener listener) {
        listeners.remove(listener);
    }

    private boolean isOK() {
        return (news != null && !news.isEmpty());
    }

    private void changeNews() {
        if ((newsIterator == null) || !newsIterator.hasNext()) {
            newsIterator = news.iterator();
        }
        currentNews = newsIterator.next();
        fireNewsFeedEvent(NewsFeedEvent.NEWS_CHANGED);
    }

    private void updateNewsFeed() {
        Calendar now = Calendar.getInstance();
        if (now.after(nextFeedUpdateTime)) {
            news = null;

            readSettings();

            if (!newsFetcher.isOK()) {
                return;
            }

            try {
                // TODO : update only if RSS feed has new news
                news = newsFetcher.fetchNews();
            } catch (Exception e) {
                System.err.println(e);
                e.printStackTrace();
                fireNewsFeedErrorEvent(UiDataMessages.NEWS_FEED_UNAVAILABLE);
            }

            nextFeedUpdateTime = now;
            nextFeedUpdateTime.add(Calendar.SECOND, feedUpdateTimeInterval);
            newsIterator = null;
        }
    }

    private void readSettings() {
        try {
            newsFetcher.readSettings();
            newsChangeTimeInterval = NewsFeedSettings.getIntSetting("newsChangeTimeInterval") * 1000;
            feedUpdateTimeInterval = NewsFeedSettings.getIntSetting("feedUpdateTimeInterval") * 1000;
        } catch (Exception e) {
            System.err.println(e);
            e.printStackTrace();
            fireNewsFeedErrorEvent(UiDataMessages.NEWS_FEED_CONFIGURATION_ERROR);
        }
    }

    private void fireNewsFeedEvent(int eventType) {
        NewsFeedEvent event = new NewsFeedEvent(this, eventType);
        Iterator<NewsFeedEventListener> iterator = listeners.iterator();
        while (iterator.hasNext()) {
            ((NewsFeedEventListener) iterator.next()).newsFeedEventReceived(event);
        }
    }

    private void fireNewsFeedErrorEvent(String errorMessage) {
        NewsFeedErrorEvent errorEvent = new NewsFeedErrorEvent(this, errorMessage);
        Iterator<NewsFeedEventListener> iterator = listeners.iterator();
        while (iterator.hasNext()) {
            ((NewsFeedEventListener) iterator.next()).newsFeedEventReceived(errorEvent);
        }
    }

}
