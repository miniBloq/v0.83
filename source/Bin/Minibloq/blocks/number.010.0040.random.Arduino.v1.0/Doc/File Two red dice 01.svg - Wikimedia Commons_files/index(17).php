/* <source lang="javascript"> Top of Javascript */
/* tooltips and access keys */
// BEGIN: MediaWiki interface button definition for Duesentrieb's image tools
// written by Dbenbenn, Avatar, Duesentrieb and Arnomane

if (typeof(load_extratabs) == 'undefined' && typeof(extraTabs) == 'undefined') {

    //Legacy:
    //Can't be move inside, unless you want to change 25+ subpages
    //Interface strings, translations get defined in the MediaWiki:Extra-tabs.js/<ISO-Code> sub pages, see below
    var tab_check_usage = 'Global usage';
    var tab_categorize = 'Find categories';
    var tab_log = 'Log';
    var tab_gallery = 'Gallery';
    var tab_orphans = 'Orphans';
    var tab_untagged = 'Untagged';
    var tab_tree = 'Tree';
    var tab_catscan = 'CatScan';
    var tab_blocklog = 'Blocklog';
    var tab_wikipedia = 'en';
    var tab_purge = 'Purge';
    var tab_thumbPurge = 'Generate thumbnail';

    if (wgUserLanguage != 'en') importScript('MediaWiki:Extra-tabs.js/' + wgUserLanguage);
    function global_append_tab(url, name, id)
    {
        addPortletLink('p-cactions', url, name, id);
    }

    var extraTabs = {
        //Constants
        tab_project: 'commons.wikimedia.org',

        // Namespace names (wgFormattedNamespaces changes depending on the wiki's content language (not interface language). So it represents exactly what we need and will work without modification on otherwikis aswell). With fallback to English namespaces which are alias'ed on all wmf-wikis anyway
        tab_ns_image: wgFormattedNamespaces["6"] +':' ,
        tab_ns_user: wgFormattedNamespaces["2"] +':' ,
        tab_ns_special: wgFormattedNamespaces["-1"] + ':' ,

        title: encodeURIComponent(wgTitle.split(" ").join("_")),

        global_add_image_tabs: function()
        {
            addPortletLink('p-cactions', wgScript + '?title=' + this.tab_ns_special + 'GlobalUsage&target=' + this.title, tab_check_usage, 'ca-checkusage');
            addPortletLink('p-cactions', 'http://toolserver.org/~daniel/WikiSense/CommonSense.php?i=' + this.title + '&go-clean=yes', tab_categorize, 'ca-commonsense');
            addPortletLink('p-cactions', wgScript + '?title=' + this.tab_ns_special + 'Log&page=' + this.tab_ns_image + this.title, tab_log, 'ca-log');
            

            //Add thumbnail purge link if user activated MediaWiki:Gadget-ThumbnailPurger.js
            if ( typeof window.thumbnailPurge !== 'undefined' ) { 
                addPortletLink('p-tb', wgScript + '?title=' + this.tab_ns_image + this.title + '&action=purge', tab_purge, 't-purge');
                addPortletLink('p-tb', wgArticlePath.replace( '$1', 'Special:FilePath' ) + '?file=' + this.title + '&width=800', tab_thumbPurge, 't-thumbpurge');
            } else {
                addPortletLink('p-cactions', wgScript + '?title=' + this.tab_ns_image + this.title + '&action=purge', tab_purge, 'ca-purge');
            }
            // Add an "unthumb" link. For File:123px-Foo.jpg, links to File:Foo.jpg.
            if (this.title.match(/^d+px-/))
            {
                this.title = this.title.replace(/^d+px-/, '');
                addPortletLink('p-cactions', wgArticlePath.replace( '$1', 'File:' + this.title ), 'Unthumb', 'ca-unthumb');
            }
            // Add a tab going to wikipedia image page
            addPortletLink('p-cactions', 'http://' + tab_wikipedia + '.wikipedia.org/wiki/File:' + this.title, tab_wikipedia, 'ca-' + tab_wikipedia + 'wiki');
        },

        global_add_user_tabs: function(title)
        {
            username = (title || this.title.split("%2F")[0]);

            addPortletLink('p-cactions', 'http://toolserver.org/~daniel/WikiSense/Gallery.php?wikifam=' + this.tab_project + '&img_user_text=' + username, tab_gallery, 'ca-gallery');
            addPortletLink('p-cactions', 'http://toolserver.org/~daniel/WikiSense/OrphanImages.php?wiki=' + this.tab_project + '&img_user_text=' + username, tab_orphans, 'ca-orphans');
            addPortletLink('p-cactions', 'http://toolserver.org/~daniel/WikiSense/UntaggedImages.php?wiki=' + this.tab_project + '&img_user_text=' + username, tab_untagged, 'ca-untagged');
        },

        global_add_category_tabs: function()
        {
            addPortletLink('p-cactions', 'http://toolserver.org/~daniel/WikiSense/CategoryIntersect.php?wiki=' + this.tab_project + '&basecat=' + this.title, tab_catscan, 'ca-catscan');
        },

        // On Special:Contributions, we add the user tabs just like for a user page.
        // On Special:Blockip, we add a blocklog tab.
        global_add_special_tabs: function()
        {
            if (wgCanonicalSpecialPageName == "Blockip")
            {
                // so we don't break after blocking
                var username = document.getElementsByName('wpBlockAddress')[0] ? document.getElementsByName('wpBlockAddress')[0].defaultValue: '';

                if (username != '')
                {
                    addPortletLink('p-cactions', wgScript + '?title=' + this.tab_ns_special + 'Log&type=block&page=' + this.tab_ns_user + encodeURIComponent(username), tab_blocklog, 'ca-blocklog');
                }
            }
            else if (wgCanonicalSpecialPageName == "Contributions")
            {
                var username = document.getElementsByName('target')[0].value;
                // Plain username, without "User:"! And encode it.
                this.global_add_user_tabs(encodeURIComponent(username));
            }
        }
    };
    $j(document).ready(function() {
        switch (wgNamespaceNumber)
        {
        case - 1:
            extraTabs.global_add_special_tabs();
            break;
        case 2:
        case 3:
            extraTabs.global_add_user_tabs();
            break;
        case 6:
            extraTabs.global_add_image_tabs();
            break;
        case 7:
            extraTabs.global_add_image_tabs();
            break;
        case 14:
            extraTabs.global_add_category_tabs();
            break;
        default:
            break;
        }     
    });
}