// Original code written by [[User:Ilmari Karonen]]
// Rewritten & extended by [[User:DieBuche]]. Botdetection and encoding fixer by [[User:Lupo]]
//
// Ajax-based replacement for [[MediaWiki:Quick-delete-code.js]]
//
// TODO: Fix problems with moves of videos
// TODO: Delete talk
if (typeof(AjaxQuickDelete) == 'undefined') {

window.AjaxQuickDelete = {

   /**
    ** Set up the AjaxQuickDelete object and add the toolbox link.  Called via $(document).ready() during page loading.
    **/
   install: function () {
      this.insertTagButtons = [{
            label: this.i18n.toolboxLinkCopyvio,
            tag: '{{copyvio|1=%PARAMETER%}}',
            talk_tag: '{\{subst:copyvionote|1=%FILE%}}',
            img_summary: 'Marking as possible copyvio because %PARAMETER%',
            talk_summary: 'Notification of possible copyright violation',
            prompt_text: this.i18n.reasonForCopyvio
         }, {
            label: this.i18n.toolboxLinkSource,
            tag: '{\{subst:nsd}}',
            talk_tag: '{\{subst:image source|1=%FILE%}}',
            img_summary: 'File has no source',
            talk_summary: '%FILE% does not have a source'

         }, {
            label: this.i18n.toolboxLinkPermission,
            tag: '{\{subst:npd}}',
            talk_tag: '{\{subst:image permission|1=%FILE%}}',
            img_summary: 'Missing permission',
            talk_summary: 'Please send a permission for %FILE% to [[COM:OTRS|OTRS]]'
         }, {
            label: this.i18n.toolboxLinkLicense,
            tag: '{\{subst:nld}}',
            talk_tag: '{\{subst:image license|1=%FILE%}}',
            img_summary: 'Missing license',
            talk_summary: '%FILE% does not have a license'
         }];
      if (window.AjaxDeleteExtraButtons) this.insertTagButtons = this.insertTagButtons.concat(window.AjaxDeleteExtraButtons);
      
      // Import stylesheet
      importStylesheet('MediaWiki:AjaxQuickDelete.css');
       
      // Don't install link if the user opted out, or when were on a special page
      if (window.AjaxDeleteOptOut || wgNamespaceNumber < 0) return;

      // Set up toolbox link
      if (wgNamespaceNumber != 14) {
         addPortletLink('p-tb', 'javascript:AjaxQuickDelete.nominateForDeletion();', this.i18n.toolboxLinkDelete, 't-ajaxquickdelete', null);
      } else {
         addPortletLink('p-tb', 'javascript:AjaxQuickDelete.discussCategory();', this.i18n.toolboxLinkDiscuss, 't-ajaxquickdiscusscat', null);
      }

      // Check user group. Attention: Array.prototype.indexOf does not exist on IE! See also
      // https://bugzilla.wikimedia.org/show_bug.cgi?id=24083 .
      if (wgUserGroups && (' ' + wgUserGroups.join(' ') + ' ').indexOf(' sysop ') != -1) {
         this.userRights = 'sysop';
      } else if (wgUserGroups && (' ' + wgUserGroups.join(' ') + ' ').indexOf(' filemover ') != -1) {
         this.userRights = 'filemover';
      }

      // Install AjaxMoveButton
      if ((this.userRights == 'filemover' || this.userRights == 'sysop') && wgNamespaceNumber == 6) {

         // Also add a "Move & Replace" button to dropdown menu
         addPortletLink('p-cactions', 'javascript:AjaxQuickDelete.moveFile("", "");', this.i18n.dropdownMove, 'ca-quickmove', 'ca-move');

         //Add quicklinks to template
         if ($('#AjaxRenameLink').length) {
            $('#AjaxRenameLink').append('<a href="javascript:AjaxQuickDelete.moveFile();">Move file and replace all usage</a>').append('<a href="javascript:AjaxQuickDelete.declineMoveFile();" class="ajaxDeleteDeclineMove"><sup> Decline request</sup></a>');
         }
      }
      if (this.userRights == 'sysop' && wgNamespaceNumber == 6) {
         if ($('#AjaxDupeProcess').length) $('#AjaxDupeProcess').append('<a href="javascript:AjaxQuickDelete.processDupes();">Process Duplicates</a>').show();
      }
      // Define optional buttons
      if (window.QuickDeleteEnhanced && wgNamespaceNumber == 6) {
         $.each(this.insertTagButtons, function (k, v) {
            addPortletLink('p-tb', 'javascript:AjaxQuickDelete.insertTagOnPage("' + v.tag + '","' + v.img_summary + '","' + v.talk_tag + '","' + v.talk_summary + '","' + v.prompt_text + '");', v.label);
         });
      }
   },
   /**
    ** For moving files
    **/
   moveFile: function () {

      if ($('#AjaxRenameLink').length) {
         this.possibleDestination = this.cleanFileName($('#AjaxRenameDestination').html());
         this.possibleReason = this.cleanReason($('#AjaxRenameReason').html());
      }

      if ($('#globalusage').length || !$('#mw-imagepage-nolinkstoimage').length) this.inUse = true;

      this.tasks = [];

      this.addTask('getMoveToken');
      this.addTask('movePage');
      this.addTask('removeTemplate');
      if (this.inUse) this.addTask('replaceUsage');

      // finally reload the page to show changed page
      this.addTask('reloadPage');

      this.prompt([{
         message: this.i18n.moveDestination,
         prefill: (this.possibleDestination || ''),
         returnvalue: 'destination',
         cleanUp: true,
         noEmpty: true
      }, {
         message: this.i18n.reasonForMove,
         prefill: (this.possibleReason || ''),
         returnvalue: 'reason',
         cleanUp: true,
         noEmpty: false
      }, {
         message: this.i18n.leaveRedirect,
         prefill: true,
         returnvalue: 'wpLeaveRedirect',
         cleanUp: false,
         noEmpty: false,
         type: 'checkbox'
      }], 'Moving file');
      if (this.inUse || this.userRights == 'filemover') $('#AjaxQuestion2').attr('disabled', true);
   },

   /**
    ** For declining request
    **/
   declineMoveFile: function () {
      // No valid reason stated, see the rename guidelines
      this.tasks = [];

      this.addTask('getMoveToken');
      this.addTask('removeTemplate');

      // finally reload the page to show the deletion tag
      this.addTask('reloadPage');

      this.prompt([{
         message: '',
         prefill: 'No valid reason stated, see the [[COM:MOVE|rename guidelines]]',
         returnvalue: 'declineReason',
         cleanUp: false,
         noEmpty: true
      }], this.i18n.declineMove);
   },

   insertTagOnPage: function (tag, img_summary, talk_tag, talk_summary, prompt_text, page) {
      this.pageName = (page === undefined) ? wgPageName.replace(/_/g,' ') : page.replace(/_/g,' ');
      this.tag = tag + '\n';
      this.img_summary = img_summary;

      // first schedule some API queries to fetch the info we need...
      this.tasks = [];

      // get token
      this.addTask('findCreator');

      this.addTask('prependTemplate');

      if (talk_tag != "undefined") {
         this.talk_tag = talk_tag.replace('%FILE%', this.pageName);
         this.talk_summary = talk_summary.replace('%FILE%', '[[:' + this.pageName + ']]');

         this.usersNeeded = true;
         this.addTask('notifyUploaders');
      }
      this.addTask('reloadPage');

      if (tag.indexOf("%PARAMETER%") != -1) {
         this.prompt([{
            message: '',
            prefill: '',
            returnvalue: 'reason',
            cleanUp: true,
            noEmpty: true
         }], prompt_text || this.i18n.reasonForDeletion);
      } else {
         this.nextTask();
      }
   },

   discussCategory: function () {
      this.pageName = wgPageName;
      this.startDate = new Date();
      this.tag = '{\{subst:cfd}}';
      this.img_summary = 'This category needs discussion';
      this.talk_tag = "{\{subst:cdw|" + wgPageName + "}}";
      this.talk_summary = "[[:" + wgPageName + "]] needs discussion";

      // set up some page names we'll need later
      this.requestPage = 'Commons:Categories for discussion/' + this.formatDate("YYYY/MM/") + wgPageName;
      this.dailyLogPage = 'Commons:Categories for discussion/' + this.formatDate("YYYY/MM");

      // first schedule some API queries to fetch the info we need...
      this.tasks = []; // reset task list in case an earlier error left it non-empty
      // ...then schedule the actual edits
      this.addTask('findCreator');
      this.addTask('notifyUploaders');
      this.addTask('prependTemplate');
      this.addTask('createRequestSubpage');
      this.addTask('listRequestSubpage');

      // finally reload the page to show the deletion tag
      this.addTask('reloadPage');

      this.prompt([{
         message: '',
         prefill: '',
         returnvalue: 'reason',
         cleanUp: true,
         noEmpty: true
      }], this.i18n.reasonForDiscussion);

   },
   nominateForDeletion: function (page) {
      this.pageName = (page === undefined) ? wgPageName : page;
      this.startDate = new Date();

      // set up some page names we'll need later
      this.requestPage = this.requestPagePrefix + this.pageName;
      this.dailyLogPage = this.requestPagePrefix + this.formatDate("YYYY/MM/DD");

      this.tag = "{{delete|reason=%PARAMETER%|subpage=" + this.pageName + this.formatDate("|year=YYYY|month=MON|day=DAY}}\n");
      this.img_summary = 'Nominating for deletion';
      this.talk_tag = "{\{subst:idw|" + this.pageName + "}}";
      this.talk_summary = "[[:" + this.pageName + "]] has been nominated for deletion";

      // first schedule some API queries to fetch the info we need...
      this.tasks = []; // reset task list in case an earlier error left it non-empty
      this.addTask('findCreator');

      // ...then schedule the actual edits
      this.addTask('prependTemplate');
      this.addTask('createRequestSubpage');
      this.addTask('listRequestSubpage');
      this.addTask('notifyUploaders');

      // finally reload the page to show the deletion tag
      this.addTask('reloadPage');

      this.prompt([{
         message: '',
         prefill: '',
         returnvalue: 'reason',
         cleanUp: true,
         noEmpty: true
      }], this.i18n.reasonForDeletion);
   },

   processDupes: function () {

      if ($('#globalusage').length || !$('#mw-imagepage-nolinkstoimage').length) this.inUse = true;
      this.tasks = []; // reset task list in case an earlier error left it non-empty
      this.addTask('getDupeDetails');

      this.addTask('compareDetails');
      this.addTask('mergeDescriptions');
      this.addTask('saveDescription');
      if (this.inUse) this.addTask('replaceUsage');
      this.addTask('deletePage');
      this.addTask('redirectPage');

      this.addTask('reloadPage');

      this.destination = $('#AjaxDupeDestination').html();
      this.nextTask();

   },
   getDupeDetails: function () {
      var query = {
         action: 'query',
         prop: 'imageinfo|revisions|info',
         rvprop: 'content|timestamp',
         intoken: 'edit|delete',
         iiprop: 'size|sha1|url',
         iiurlwidth: 365,
         titles: wgPageName + '|' + this.destination
      };
      this.doAPICall(query, 'getDupeDetailsCB');
      this.showProgress('Fetching details');
   },
   getDupeDetailsCB: function (result) {
      var pages = result.query.pages;
      this.details = [];
      i = 0;

      for (var id in pages) {
         v = pages[id];
         n = this.details[i] = {};
         n.title = v.title;
         n.size = v.imageinfo[0].size;
         n.width = v.imageinfo[0].width;
         n.height = v.imageinfo[0].height;
         n.thumburl = v.imageinfo[0].thumburl;
         n.sha1 = v.imageinfo[0].sha1;
         n.content = v.revisions[0]['*'];
         n.starttimestamp = v.starttimestamp;
         this.edittoken = v.edittoken;
         this.deletetoken = v.deletetoken;
         i++;
      }
      //If ordner (old=0, new=1) not correct: Reverse the order
      if (this.details[0].title != wgPageName.replace(/_/g, ' ')) this.details.reverse();
      this.nextTask();
   },

   /**
    ** Edit the current page to add the specified tag.  Assumes that the page hasn't
    ** been tagged yet; if it is, a duplicate tag will be added.
    **/
   prependTemplate: function () {
      var page = [];
      page.title = this.pageName;
      page.text = this.tag;
      page.editType = 'prependtext';
      if (window.AjaxDeleteWatchFile) page.watchlist = 'watch';

      this.showProgress(this.i18n.addingAnyTemplate);
      this.savePage(page, this.img_summary, 'nextTask');
   },

   /**
    ** Create the DR subpage (or append a new request to an existing subpage).
    ** The request page will always be watchlisted.
    **/
   createRequestSubpage: function () {
      this.templateAdded = true; // we've got this far; if something fails, user can follow instructions on template to finish
      var page = [];
      page.title = this.requestPage;
      page.text = "\n\n=== [[:" + this.pageName + "]] ===\n" + this.reason + " ~~" + "~~\n";
      page.watchlist = 'watch';
      page.editType = 'appendtext';

      this.showProgress(this.i18n.creatingNomination);

      this.savePage(page, "Starting deletion request", 'nextTask');
   },

   /**
    ** Transclude the nomination page onto today's DR log page, creating it if necessary.
    ** The log page will never be watchlisted (unless the user is already watching it).
    **/
   listRequestSubpage: function () {
      var page = [];
      page.title = this.dailyLogPage;

      // Impossible when using appendtext. Shouldn't not be severe though, since DRBot creates those pages before they are needed.
      // if (!page.text) page.text = "{{"+"subst:" + this.requestPagePrefix + "newday}}";  // add header to new log pages
      page.text = "\n{{" + this.requestPage + "}}\n";
      page.watchlist = 'nochange';
      page.editType = 'appendtext';

      this.showProgress(this.i18n.listingNomination);

      this.savePage(page, "Listing [[" + this.requestPage + "]]", 'nextTask');
   },

   /**
    ** Notify any uploaders/creators of this page using {{idw}}.
    **/
   notifyUploaders: function () {
      this.uploadersToNotify = 0;
      for (var user in this.uploaders) {
         if (user == wgUserName) continue; // notifying yourself is pointless
         var page = [];
         page.title = this.userTalkPrefix + user;
         page.text = "\n" + this.talk_tag + " ~~" + "~~\n";
         page.editType = 'appendtext';
         page.redirect = true;
         if (window.AjaxDeleteWatchUserTalk) page.watchlist = 'watch';
         this.savePage(page, this.talk_summary, 'uploaderNotified');

         this.showProgress(this.i18n.notifyingUploader.replace('%USER%', user));

         this.uploadersToNotify++;
      }
      if (this.uploadersToNotify === 0) this.nextTask();
   },

   uploaderNotified: function () {
      this.uploadersToNotify--;
      if (this.uploadersToNotify === 0) this.nextTask();
   },

   /**
    ** Compile a list of uploaders to notify.  Users who have only reverted the file to an
    ** earlier version will not be notified.
    ** DONE: notify creator of non-file pages
    **/
   findCreator: function () {
      if (wgNamespaceNumber == 6) {
         var query = {
            action: 'query',
            prop: 'imageinfo|revisions|info',
            rvprop: 'content|timestamp',
            intoken: 'edit',
            iiprop: 'user|sha1|comment',
            iilimit: 50,
            titles: this.pageName
         };

      } else {
         var query = {
            action: 'query',
            prop: 'info|revisions',
            rvprop: 'user|timestamp',
            rvlimit: 1,
            rvdir: 'newer',
            intoken: 'edit',
            titles: this.pageName
         };
      }
      this.showProgress();
      this.doAPICall(query, 'findCreatorCB');
   },
   findCreatorCB: function (result) {
      this.uploaders = {};
      var pages = result.query.pages;
      for (var id in pages) { // there should be only one, but we don't know its ID
         // The edittoken only changes between logins
         this.edittoken = pages[id].edittoken;

         //First handle non-file pages
         if (wgNamespaceNumber != 6) {

            this.pageCreator = pages[id].revisions[0].user;
            this.starttimestamp = pages[id].starttimestamp;
            this.timestamp = pages[id].revisions[0].timestamp;

            this.uploaders[this.pageCreator] = true;

         } else {
            var info = pages[id].imageinfo;

            var content = pages[id].revisions[0]['*'];

            var seenHashes = {};
            for (var i = info.length - 1; i >= 0; i--) { // iterate in reverse order
               if (info[i].sha1 && seenHashes[info[i].sha1]) continue; // skip reverts
               // Now exclude bots which only reupload a new version:
               this.excludedBots = 'FlickreviewR, Rotatebot, Cropbot, Picasa Review Bot';
               if (this.excludedBots.indexOf(info[i].user) != -1) continue;

               // Handle some special cases, most of the code by [[User:Lupo]]
               if (info[i].user == 'File Upload Bot (Magnus Manske)') {
                  // CommonsHelper
                  match = /transferred to Commons by \[\[User:([^\]\|]*)(\|([^\]]*))?\]\] using/.exec(info[i].comment);

                  // geograph_org2commons, regex accounts for typo ("transferd") and it's possible future correction
                  if (!match) match = /geograph.org.uk\]; transferr?e?d by \[\[User:([^\]\|]*)(\|([^\]]*))?\]\] using/.exec(info[i].comment);

                  // flickr2commons
                  if (!match) match = /\* Uploaded by \[\[User:([^\]\|]*)(\|([^\]]*))?\]\]/.exec(content);

                  if (match) match = match[1];
                  // Really necessary?
                  match = this.fixDoubleEncoding(match);
               } else if (info[i].user == 'FlickrLickr') {
                  match = /\n\|reviewer=\s*(.*)\n/.exec(content);
                  if (match) match = match[1];
               } else if (info[i].user == 'Flickr upload bot') {
                  // Check for the bot's upload template
                  match = /\{\{User:Flickr upload bot\/upload(\|[^\|\}]*)?\|reviewer=([^\}]*)\}\}/.exec(content);
                  if (match) match = match[2];
               } else {
                  // No special case applies, just continue;
                  this.uploaders[info[i].user] = true;
                  continue;
               }
               if (match) {
                  // Make sure the username is in canonical form
                  match = match.replace(/^[\s_]+/, "").replace(/[\s_]+$/, "").replace(/[\s_]+/g, " ");
                  match = match.substr(0, 1).toUpperCase() + match.substr(1);
                  this.uploaders[match] = true;
               }
            }
         }
      }
      this.nextTask();
   },

   getMoveToken: function () {
      var query = {
         action: 'query',
         prop: 'info|revisions',
         rvprop: 'content|timestamp',
         intoken: 'edit|move',
         titles: wgPageName
      };

      this.showProgress();
      this.doAPICall(query, 'getMoveTokenCB');
   },

   getMoveTokenCB: function (result) {
      var pages = result.query.pages;
      for (var id in pages) { // there should be only one, but we don't know its ID
         // The edittoken only changes between logins
         this.edittoken = pages[id].edittoken;
         this.movetoken = pages[id].movetoken;
         this.pageContent = pages[id].revisions[0]['*'];
         this.starttimestamp = pages[id].starttimestamp;
         this.timestamp = pages[id].revisions[0].timestamp;
      }

      this.nextTask();
   },

   removeTemplate: function () {
      var page = [];
      page.title = (this.destination || wgPageName);
      page.text = this.pageContent.replace(/\{\{(rename|rename media|move)\|.*?\}\}/i, "");
      page.editType = 'text';
      page.starttimestamp = this.starttimestamp;
      page.timestamp = this.timestamp;

      this.showProgress(this.i18n.removingTemplate);
      this.savePage(page, (this.declineReason || this.i18n.renameDone), 'nextTask');
   },

   replaceUsage: function () {
      var page = [];
      page.title = 'User:CommonsDelinker/commands';
      if (this.userRights == 'filemover') page.title = 'User talk:CommonsDelinker/commands';
      if (!this.details) this.reason = '[[Commons:File renaming|File renamed]]: ' + this.reason;
      page.text = '\n{{universal replace|' + wgPageName.replace('File:', '') + '|' + this.destination.replace('File:', '') + '|reason=' + this.reason + '}}';
      page.editType = 'appendtext';

      this.showProgress(this.i18n.replacingUsage);
      this.savePage(page, 'universal replace: [[:' + wgPageName + ']] → [[:' + this.destination + ']]', 'nextTask');
   },
   redirectPage: function () {
      var page = [];
      page.title = wgPageName;
      page.text = '#REDIRECT [[' + this.destination + ']]';
      page.editType = 'text';

      this.showProgress(this.i18n.redirectingFile);
      this.savePage(page, 'Redirecting to duplicate file', 'nextTask');
   },
   saveDescription: function () {
      var page = [];
      page.title = this.destination;
      page.text = this.newPageText;
      page.editType = 'text';

      this.showProgress(this.i18n.savingDescription);
      this.savePage(page, 'Merging details from duplicate', 'nextTask');
   },


   /**
    ** Pseudo-Modal JS windows.
    **/
   prompt: function (questions, title, width) {
      var dlgButtons = {};
      dlgButtons[this.i18n.submitButtonLabel] = function () {
         $.each(questions, function (i, v) {
            response = $('#AjaxQuestion' + i).val();
            if (v.type == 'checkbox') response = $('#AjaxQuestion' + i).attr('checked');
            if (v.cleanUp) {
               if (v.returnvalue == 'reason') response = AjaxQuickDelete.cleanReason(response);
               if (v.returnvalue == 'destination') response = AjaxQuickDelete.cleanFileName(response);
            }
            AjaxQuickDelete[v.returnvalue] = response;
            if (v.returnvalue == 'reason' && AjaxQuickDelete.tag) {
               AjaxQuickDelete.tag = AjaxQuickDelete.tag.replace('%PARAMETER%', response);
               AjaxQuickDelete.img_summary = AjaxQuickDelete.img_summary.replace('%PARAMETER%', response);
               AjaxQuickDelete.img_summary = AjaxQuickDelete.img_summary.replace('%PARAMETER-LINKED%', '[[:' + response + ']]');
            }
         });
         $(this).dialog('close');
         AjaxQuickDelete.nextTask();
      };
      dlgButtons[this.i18n.cancelButtonLabel] = function () {
         $(this).dialog('close');
      };

      var $dialog = $('<div></div>').html('<div id="AjaxDeleteContainer"></div>').dialog({
         width: (width || 600),
         modal: true,
         title: title,
         draggable: false,
         dialogClass: "wikiEditor-toolbar-dialog",
         close: function () {
            $(this).dialog("destroy");
            $(this).remove();
         },
         buttons: dlgButtons
      });
      var submitButton = $('.ui-dialog-buttonpane button:first');

      $.each(questions, function (i, v) {
         if (v.type == 'textarea') {
            $('#AjaxDeleteContainer').append(v.message).append('<textarea rows=20 id="AjaxQuestion' + i + '"><br><br>');
         } else {
            $('#AjaxDeleteContainer').append(v.message).append('<input type="' + (v.type || 'text') + '" id="AjaxQuestion' + i + '" style="width: 98%;"><br><br>');
         }
         var curQuestion = $('#AjaxQuestion' + i);
         curQuestion.keyup(function (event) {
            if (v.noEmpty) {
               if ($(this).val().length < 4) {
                  submitButton.addClass('ui-state-disabled');
               } else {
                  submitButton.removeClass('ui-state-disabled');
               }
            }
            if (event.keyCode == '13' && v.enterToSubmit != false) submitButton.click();
         });
         curQuestion.val(v.prefill);
         if (v.type == 'checkbox') $('#AjaxQuestion' + i).attr('checked', v.prefill).attr('style', 'margin-left: 5px');
         curQuestion.keyup();
      });
      $('#AjaxQuestion0').focus();
   },

   /**
    ** Pseudo-Modal JS windows.
    **/
   compareDetails: function () {
      d = this.details[0];
      f = this.details[1];
      document.body.style.cursor = 'default';
      this.progressDialog.remove();
      if (d.sha1 == f.sha1) {
         this.exactDupes = true;
         this.nextTask();
         return;
      }
      var dlgButtons = {};
      dlgButtons[this.i18n.submitButtonLabel] = function () {

         $(this).dialog("close");
         AjaxQuickDelete.nextTask();
      };
      dlgButtons[this.i18n.cancelButtonLabel] = function () {
         $(this).dialog("close");
      };
      var $dialog = $('<div></div>').html('<div id="AjaxDupeContainer"></div>').dialog({
         width: 800,
         modal: true,
         title: 'title',
         draggable: false,
         dialogClass: "wikiEditor-toolbar-dialog",
         close: function () {
            $(this).dialog("destroy");
            $(this).remove();
         },
         buttons: dlgButtons
      });
      $('#AjaxDupeContainer').append('<div><img src="' + d.thumburl + '" ></div><div><img src="' + f.thumburl + '"></div><br>').append('<div>' + Math.round(d.size / 1000) + ' KB <br>' + d.width + 'x' + d.height + '</div>').append('<div>' + Math.round(f.size / 1000) + ' KB <br>' + f.width + 'x' + f.height + '</div>');
   },

   mergeDescriptions: function () {
      this.prompt([{
         message: '',
         prefill: this.details[0].content,
         returnvalue: 'discard',
         cleanUp: false,
         noEmpty: false,
         type: 'textarea',
         enterToSubmit: false
      }, {
         message: '',
         prefill: this.details[1].content,
         returnvalue: 'newPageText',
         cleanUp: false,
         noEmpty: false,
         type: 'textarea',
         enterToSubmit: false
      }], this.i18n.mergeDescription, 800);
      this.destination = this.details[1].title;
      this.reason = 'Exact or scaled-down duplicate: [[:' + this.destination + ']]';
   },
   /**
    ** Double encoding fixer by Lupo. This is necessary for some older uploads of Magnus' bot.
    **/
   fixDoubleEncoding: function (match) {
      if (!match) return match;
      var utf8 = /[u00C2-u00F4][u0080-u00BF][u0080-u00BF]?[u0080-u00BF]?/g;
      if (!utf8.test(match)) return match;
      // Looks like we have a double encoding. At least it contains character
      // sequences that might be legal UTF-8 encodings. Translate them into %-
      // syntax and try to decode again.
      var temp = "",
          curr = 0,
          m, hex_digit = "0123456789ABCDEF";
      var str = match.replace(/%/g, '%25');
      utf8.lastIndex = 0;
      // Reset regexp to beginning of string
      try {
         while ((m = utf8.exec(str)) != null) {
            temp += str.substring(curr, m.index);
            m = m[0];
            for (var i = 0; i < m.length; i++) {
               temp += '%' + hex_digit.charAt(m.charCodeAt(i) / 16) + hex_digit.charAt(m.charCodeAt(i) % 16);
            }
            curr = utf8.lastIndex;
         }
         if (curr < str.length) temp += str.substring(curr);
         temp = decodeURIComponent(temp);
         return temp;
      } catch (e) {}
      return match;
   },

   cleanFileName: function (uncleanName) {
      uncleanName = uncleanName.replace(/^Image:/i, 'File:');
      uncleanName = uncleanName.replace(/\.jpe*g$/i, '.jpg');
      uncleanName = uncleanName.replace(/\.png$/i, '.png');
      uncleanName = uncleanName.replace(/\.gif$/i, '.gif');

      currentExtension = wgPageName.toLowerCase().replace(/.*?\.(\w{3,4})$/, '$1').replace('jpeg', 'jpg');

      // If new file name is without extension, add the one from the old name
      if (uncleanName.toLowerCase().indexOf(currentExtension) == -1) uncleanName += '.' + currentExtension;
      if (uncleanName.indexOf('File:') == -1) uncleanName = 'File:' + uncleanName;
      return uncleanName;
   },
   cleanReason: function (uncleanReason) {
      // trim whitespace
      uncleanReason = uncleanReason.replace(/^\s*(.+)\s*$/, '$1');
      // remove signature
      uncleanReason = uncleanReason.replace(/(.+)(--)?~{3,5}$/, '$1');
      return uncleanReason;
   },

   /**
    ** For display of progress messages.
    **/
   showProgress: function (message) {
      if ($('#feedbackContainer').length) {
         $('#feedbackContainer').html(message);
      } else {
         document.body.style.cursor = 'wait';

         this.progressDialog = $('<div></div>').html('<div id="feedbackContainer">' + (message || this.i18n.preparingToEdit) + '</div>').dialog({
            width: 450,
            height: 90,
            minHeight: 90,
            modal: true,
            resizable: false,
            draggable: false,
            closeOnEscape: false,
            dialogClass: "ajaxDeleteFeedback"
         });
         $('.ui-dialog-titlebar').hide();
      }

   },
   /**
    ** Submit an edited page.
    **/
   savePage: function (page, summary, callback) {
      var edit = {
         action: 'edit',
         summary: summary,
         watchlist: (page.watchlist || 'preferences'),
         title: page.title,
         token: this.edittoken
      };
      if (page.redirect) edit.redirect = '';
      edit[page.editType] = page.text;
      this.doAPICall(edit, callback);
   },

   movePage: function () {
      var edit = {
         action: 'move',
         reason: this.reason,
         from: wgPageName,
         to: this.destination,
         movetalk: '',
         token: this.movetoken
      };
      // Option to not leave a redirect behind, MediaWiki default does leave one behind
      // Just like movetalk, an empty parameter sets it to true (true to not leave a redirect behind)
      if (this.wpLeaveRedirect === false) {
         edit.noredirect = '';
      }

      this.showProgress(this.i18n.movingFile);
      this.doAPICall(edit, 'nextTask');
   },

   deletePage: function () {
      var edit = {
         action: 'delete',
         reason: this.reason,
         title: wgPageName,
         token: this.deletetoken,
         recreate: ''
      };
      this.showProgress(this.i18n.deletingFile);
      this.doAPICall(edit, 'nextTask');
   },

   /**
    ** Does a MediaWiki API request and passes the result to the supplied callback (method name).
    ** Uses POST requests for everything for simplicity.
    **/
   doAPICall: function (params, callback) {
      var o = this;

      params.format = 'json';
      $.ajax({
         url: this.apiURL,
         cache: false,
         dataType: 'json',
         data: params,
         type: 'POST',
         success: function (result, status, x) {
            if (!result) return o.fail("Receive empty API response:\n" + x.responseText);

            // In case we get the mysterious 231 unknown error, just try again
            if (result.error && result.error.info.indexOf('231') != -1) return setTimeout(function () {
               o.doAPICall(params, callback);
            }, 500);
            if (result.error) return o.fail("API request failed (" + result.error.code + "): " + result.error.info);
            if (result.edit && result.edit.spamblacklist) return o.fail("The edit failed because " + result.edit.spamblacklist + " is on the Spam Blacklist");
            try {
               o[callback](result);
            } catch (e) {
               return o.fail(e);
            }
         },
         error: function (x, status, error) {
            return o.fail("API request returned code " + x.status + " " + status + "Error code is " + error);
         }
      });
   },

   /**
    ** Simple task queue.  addTask() adds a new task to the queue, nextTask() executes
    ** the next scheduled task.  Tasks are specified as method names to call.
    **/
   tasks: [],
   // list of pending tasks
   currentTask: '',
   // current task, for error reporting
   addTask: function (task) {
      this.tasks.push(task);
   },
   nextTask: function () {
      var task = this.currentTask = this.tasks.shift();
      try {
         this[task]();
      } catch (e) {
         this.fail(e);
      }
   },

   /**
    ** Once we're all done, reload the page.
    **/
   reloadPage: function () {
      this.progressDialog.remove();
      if (this.pageName && this.pageName.replace(/ /g, '_') != wgPageName) return;
      var encTitle = (this.destination || wgPageName);
      encTitle = encodeURIComponent(encTitle.replace(/ /g, '_')).replace(/%2F/ig, '/').replace(/%3A/ig, ':');
      location.href = wgServer + wgArticlePath.replace("$1", encTitle);
   },

   /**
    ** Crude error handler. Just throws an alert at the user and
    ** (if we managed to add the {{delete}} tag) reloads the page.
    **/
   fail: function (err) {
      document.body.style.cursor = 'default';
      var msg = this.i18n.taskFailure[this.currentTask] || this.i18n.genericFailure;

      //TODO: Needs cleanup
      if (this.img_summary == 'Nominating for deletion') fix = (this.templateAdded ? this.i18n.completeRequestByHand : this.i18n.addTemplateByHand);
      else fix = '';

      $('#feedbackContainer').html(msg + " " + fix + "<br>" + this.i18n.errorDetails + "<br>" + err + "<br><a href=" + wgServer + "/wiki/MediaWiki_talk:AjaxQuickDelete.js>" + this.i18n.errorReport + "</a>");
      $('.ui-dialog-content').height('auto');
      $('.ui-dialog').addClass('ajaxDeleteError');
      // Allow some time to read the message
      if (this.templateAdded) setTimeout(this.reloadPage(), 5000);
   },

   /**
    ** Very simple date formatter.  Replaces the substrings "YYYY", "MM" and "DD" in a
    ** given string with the UTC year, month and day numbers respectively.
    ** Also replaces "MON" with the English full month name and "DAY" with the unpadded day.
    **/
   formatDate: function (fmt, date) {
      var pad0 = function (s) {
         s = "" + s;
         return (s.length > 1 ? s : "0" + s);
      }; // zero-pad to two digits
      if (!date) date = this.startDate;
      fmt = fmt.replace(/YYYY/g, date.getUTCFullYear());
      fmt = fmt.replace(/MM/g, pad0(date.getUTCMonth() + 1));
      fmt = fmt.replace(/DD/g, pad0(date.getUTCDate()));
      fmt = fmt.replace(/MON/g, this.months[date.getUTCMonth()]);
      fmt = fmt.replace(/DAY/g, date.getUTCDate());
      return fmt;
   },
   months: "January February March April May June July August September October November December".split(" "),

   // Constants
   // DR subpage prefix
   requestPagePrefix: "Commons:Deletion requests/",
   // user talk page prefix
   userTalkPrefix: wgFormattedNamespaces[3] + ":",
   // MediaWiki API script URL
   apiURL: wgServer + wgScriptPath + "/api.php",

   // Translatable strings
   i18n: {
      toolboxLinkDelete: "Nominate for deletion",
      toolboxLinkDiscuss: "Nominate category for discussion",

      // GUI reason prompt form
      reasonForDeletion: "Why should this file be deleted?",
      reasonForDiscussion: "Why does this category need discussion?",
      submitButtonLabel: "Proceed",
      cancelButtonLabel: "Cancel",

      // GUI progress messages
      preparingToEdit: "Preparing to edit pages... ",
      creatingNomination: "Creating nomination page... ",
      listingNomination: "Adding nomination page to daily list... ",
      addingAnyTemplate: "Adding template to " + wgCanonicalNamespace.toLowerCase() + " description page... ",
      notifyingUploader: "Notifying %USER%... ",

      // Extended version
      toolboxLinkSource: "No source",
      toolboxLinkLicense: "No license",
      toolboxLinkPermission: "No permission",
      toolboxLinkCopyvio: "Report copyright violation",
      reasonForCopyvio: "Why is this file a copyright violation?",

      // For moving files
      renameDone: "Removing template; rename done",
      removingTemplate: "Removing rename template",
      notAllowed: "You do not have the neccessary rights to move files",
      reasonForMove: "Why do you want to move this file?",
      moveDestination: "What should be the new file name?",
      movingFile: "Moving file",
      replacingUsage: "Ordering CommonsDelinker to replace all usage",
      declineMove: "Why do you want to decline the request?",
      dropdownMove: "Move & Replace",
      leaveRedirect: "Leave a redirect behind:",

      //For Duplicates
      deletingFile: "Deleting file",
      mergeDescription: "Please now merge the file descriptions",
      redirectingFile: "Redirecting file",
      savingDescription: "Saving new details",

      // Errors
      genericFailure: "An error occurred while trying to do the requested action. ",
      taskFailure: {
         listUploaders: "An error occurred while determining the " + (6 == wgNamespaceNumber ? " uploader(s) of this file" : "creator of this page") + ".",
         loadPages: "An error occurred while preparing to nominate this " + wgCanonicalNamespace.toLowerCase() + " for deletion.",
         prependDeletionTemplate: "An error occurred while adding the {{delete}} template to this " + wgCanonicalNamespace.toLowerCase() + ".",
         createRequestSubpage: "An error occurred while creating the request subpage.",
         listRequestSubpage: "An error occurred while adding the deletion request to today's log.",
         notifyUploaders: "An error occurred while notifying the " + (6 == wgNamespaceNumber ? " uploader(s) of this file" : "creator of this page") + "."
      },
      addTemplateByHand: "To nominate this " + wgCanonicalNamespace.toLowerCase() + " for deletion, please edit the page to add the {{delete}} template and follow the instructions shown on it.",
      completeRequestByHand: "Please follow the instructions on the deletion notice to complete the request.",
      errorDetails: "A detailed description of the error is shown below:",
      errorReport: "Report the error here"
   }
};

if (wgUserLanguage != 'en') importScript('MediaWiki:AjaxQuickDelete.js/' + wgUserLanguage);
mediaWiki.loader.using('jquery.ui.dialog', function () {
   $(document).ready(function () {
      AjaxQuickDelete.install();
   });
});


} // end if (guard)