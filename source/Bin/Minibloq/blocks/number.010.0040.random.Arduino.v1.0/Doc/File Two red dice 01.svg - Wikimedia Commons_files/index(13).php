/***** ajax translations ********
 * Ajax translation of layout templates (licenses templates)
 * See talk page for documentation.
 *
 * Maintainers: [[User:ערן]], [[User:Ilmari Karonen]], [[User:DieBuche]]
 * Last update: 2010-10-10
 */
// <source lang="JavaScript">
if (typeof(AjaxTranslations) == 'undefined') {

    var AjaxTranslations = {
        updateLangLinks: function() {
            if (typeof (window.disableAjaxTranslation) != 'undefined') return; 
            var langLinkRgx = new RegExp("^(" +
            wgServer.replace(/([^a-z0-9])/ig, "\\$1") +
            "|)" +
            wgArticlePath.replace(/([^a-z0-9])/ig, "\\$1").
            replace("\\$1", "(Template:.*/[a-z]{2,3}(-[a-z]+)?)") +
            "$");
            $j('div.layouttemplate a, table.layouttemplate a').each(function(index) {
                var m = langLinkRgx.exec($j(this).attr('href'));

                //No translation link, skip it
                if (m) {
                   $j(this).attr('title', decodeURIComponent(m[2]).replace(/_/g, " "));
                   $j(this).click(function(evt) {
                       evt.preventDefault();
                       AjaxTranslations.loadAjaxTranslation(evt);
                   });
                }
            });
        },

        loadAjaxTranslation: function(evt) {
            var linkElement = evt.target;
            var templateName = linkElement.title;
            if (!templateName) return true;
            templateName = templateName.replace(/[\s_]+/g, "_");
            var templateParts = /^Template:(.*)\/([a-z]{2,3}(-[a-z]+)?)$/.exec(templateName);
            if (!templateParts || !templateParts.length) return true;

            this.lastLayoutTemplate = $j('[title='+ templateName.replace(/_/g, ' ') +']').parents('.layouttemplate');

            // try to find encoded template args, if supplied (EXPERIMENTAL)
            var templateArgs = "";
            this.lastLayoutTemplate.find('.layouttemplateargs').each(function (index){
              args = this.title.split(/\s+/);
              args.shift();
              $j.each(args, function (i, k){
                if (!/\S/.test(k)) return true;
                templateArgs += "|" + decodeURIComponent(k.replace(/\+/g, " ")).replace(/\|/g, "{{!}}");
              });
            });

            // {{urlencode:}} turns parser extension tags into garbage; we can't undo it, so we just give up if it's happened
            if (/\x7FUNIQ/.test(templateArgs)) templateArgs = '';

            aT=this;
            $j.ajax({
              url: wgServer + wgScriptPath + '/api.php',
              cache: false,
              dataType: 'json',
              data: {
                action : 'parse',
                format : 'json',
                prop : 'text',
                text : '{{' + templateName + templateArgs + '}}',
                title : wgPageName,
                uselang : templateParts[2]
              },
              type: 'POST',
              success: function(result, status, x) {
                if (result && result.parse && result.parse.text["*"]) {
                  aT.lastLayoutTemplate.replaceWith(result.parse.text["*"]);
                  $j('.translatedTag').hide();
                  aT.updateLangLinks();
                }
              }
            });

            if (evt.preventDefault) evt.preventDefault();
            return (evt.returnValue = false);
        }
    };
    $j(document).ready(function() { AjaxTranslations.updateLangLinks(); });
}
//</source>