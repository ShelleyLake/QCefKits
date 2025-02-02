/****************************************************************************
** QCefKits Library
** Copyright (C) panuins
** Contact: pl_014@163.com
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the LGPLV2 or later.
****************************************************************************/

#ifndef CEFWEBPAGE_H
#define CEFWEBPAGE_H
#include "ClientHandler.h"
#include <include/cef_client.h>
#include <QMap>
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <QUrl>

class QCefWidget;

class CefWebPage : public QObject, public CefHandler::ClientHandler::Delegate
{
    Q_OBJECT
public:
    struct PageFeatures
    {
        QPointer<CefWebPage> parentPage;
        int x;
        bool xSet;
        int y;
        bool ySet;
        int width;
        bool widthSet;
        int height;
        bool heightSet;
        bool menuBarVisible;
        bool statusBarVisible;
        bool toolBarVisible;
        bool scrollbarsVisible;
        bool isPopup;
    };
    ~CefWebPage();

    static QSharedPointer<CefWebPage> createNewPage(QCefWidget *w, const QUrl &url);

    void createBrowser(const QUrl &url);
    void loadUrl(const QUrl &url);

    // Called when the browser is created.
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;

    // Called when the browser is closing.
    virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) override;

    // Called when the browser has been closed.
    virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) override;

    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                  cef_log_severity_t level,
                                  const CefString& message,
                                  const CefString& source,
                                  int line) override;
    // Set the window URL address.
    virtual void OnSetAddress(CefRefPtr<CefBrowser> browser, const std::string& url) override;

    // Set the window title.
    virtual void OnSetTitle(CefRefPtr<CefBrowser> browser, const std::string& title) override;

    // Set the Favicon image.
    virtual void OnSetFavicon(CefRefPtr<CefBrowser> browser, CefRefPtr<CefImage> image) override;

    // Set fullscreen mode.
    virtual void OnSetFullscreen(CefRefPtr<CefBrowser> browser, bool fullscreen) override;

    // Auto-resize contents.
    virtual void OnAutoResize(CefRefPtr<CefBrowser> browser, const CefSize& new_size) override;

    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefLoadHandler::TransitionType transition_type) override;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode) override;
    // Set the loading state.
    virtual void OnSetLoadingState(CefRefPtr<CefBrowser> browser,
                                   bool isLoading,
                                   bool canGoBack,
                                   bool canGoForward) override;
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefLoadHandler::ErrorCode errorCode,
                             const CefString& errorText,
                             const CefString& failedUrl) override;

    // Set the draggable regions.
    virtual void OnSetDraggableRegions(
            CefRefPtr<CefBrowser> browser,
            const std::vector<CefDraggableRegion>& regions) override;

    // Set focus to the next/previous control.
    virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;

    // Called on the UI thread before a context menu is displayed.
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model) override;

    virtual QSharedPointer<CefHandler::ClientHandler::Delegate> CreatePopupWindow(
            CefRefPtr<CefBrowser> browser,
            bool is_devtools,
            CefLifeSpanHandler::WindowOpenDisposition target_disposition,
            const CefPopupFeatures& popupFeatures,
            CefWindowInfo& windowInfo,
            CefBrowserSettings& settings) override;

    void resizeBrowser(const QSize &size);
    QPointer<QCefWidget> cefWidget() const
    {
        return m_widget;
    }
    void setCefWidget(QPointer<QCefWidget> widget);

    struct PageFeatures pageFeatures;

Q_SIGNALS:
    void urlChanged(QUrl url);
    void titleChanged(QString title);
    void browserCreated();
    void browserClosed();
    void loadStarted();
    void loadComplete();
    void loadFailed();
    void newBrowserRequest(QSharedPointer<CefWebPage> page);
    void consoleMessage(QCefKits_logLevel level,
                        const QString& message,
                        const QString& source,
                        int line);

private:
    explicit CefWebPage();

    CefRefPtr<CefBrowser> m_browser;
    CefRefPtr<CefHandler::ClientHandler> m_handler;
    QPointer<QCefWidget> m_widget;
};

#endif // CEFWEBPAGE_H
