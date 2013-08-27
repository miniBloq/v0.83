/**
 * to benefit of [[:Template:Multilingual description]]
 *
 * Implements language selection for multilingual elements
 *
 * In certain environments, it's not feasible to neatly box away each
 * different language into its own section of the site. By marking elements
 * multilingual, you can emulate this behavior by only displaying the
 * message in the user's language. This reduces the "Tower of Babel" effect.
 *
 * @author Edward Z. Yang (Ambush Commander), Rewritten by DieBuche
 */

/* Configuration: */

// in your monobook.js, set ls_enable = false to stop the javascript
// the cookie name we use to stash the info.
mediaWiki.loader.using('jquery.cookie', function() 
{
var multilingual =
{
 cookie: 'commonswiki_language_js',

 // link to the language select page
 helpUrl: 'http://meta.wikimedia.org/wiki/Meta:Language_select',

 // strings that are part of the widgets
 stringHelp: {
  'cs': 'Výběr jazyka:',
  'da': 'Vælg sprog:',
  'de': 'Sprachauswahl:',
  'en': 'Language select:',
  'eo': 'Lingvoelekto:',
  'fr': 'Selecteur de langue:',
  'hu': 'Nyelvválasztás:',
  'ko': '언어 선택:',
  'mk': 'Јазик:',
  'ml': 'ഭാഷ തിരഞ്ഞെടുക്കുക:',
  'nds': 'Spraakutwahl:',
  'nl': 'Taal:',
  'pl': 'Wybierz język:',
  'pt': 'Seleção do idioma:',
  'pt-br': 'Seleção do idioma:',
  'ru': 'Выбор языка:',
  'sv': 'Välj språk:'
 },
 stringSelect: {
  'cs': 'Vybrat',
  'da': 'Vælg',
  'de': 'Auswahl',
  'en': 'Select',
  'eo': 'Elekti',
  'fr': 'Seulement ce langage',
  'hu': 'Választás',
  'ko': '선택',
  'mk': 'Одбери',
  'ml': 'തിരഞ്ഞെടുക്കുക',
  'nds': 'Utwählen',
  'nl': 'Selecteer',
  'pl': 'Wybierz',
  'pt': 'Selecionar',
  'pt-br': 'Selecionar',
  'ru': 'Выбрать',
  'sv': 'Välj'
 },
 stringShowAll: {
  'cs': 'Zobrazit všechny',
  'da': 'Vis alle',
  'de': 'Alle anzeigen',
  'en': 'Show all',
  'eo': 'ĉiuj',
  'fr': 'Tous les langages',
  'hu': 'Mutasd mind',
  'ko': '모두 보기',
  'mk': 'Сите',
  'ml': 'എല്ലാം',
  'nds': 'All wiesen',
  'nl': 'Toon alles',
  'pl': 'Pokaż wszystkie',
  'pt': 'Mostrar todos',
  'pt-br': 'Mostrar todos',
  'ru': 'Показать все',
  'sv': 'Visa alla'
 },


 // autodetects a browser language
 getBrowserLanguage: function () {
  return navigator.userLanguage || navigator.language || navigator.browserLanguage;
 },

 // sets a new language to the cookie
 setCookieLanguage: function (language) {
  jQuery.cookie(this.cookie, escape(language), {
   expires: 100,
   path: '/'
  });
 },

 // deletes the cookie
 deleteCookieLanguage: function (language) {
  jQuery.cookie(this.cookie, null, {
   path: '/'
  });
 },
 // grabs the ISO 639 language code based
 // on either the browser or a supplied cookie
 getLanguage: function () {
  var language = '';

  // Priority:
  //  1. Cookie
  //  2. wgUserLanguage global variable
  //  3. Browser autodetection
  // grab according to cookie
  language = jQuery.cookie(this.cookie);

  // grab according to wgUserLanguage if user is logged in
  if (!language && window.wgUserLanguage && wgUserGroups !== null) {
   language = wgUserLanguage;
  }

  // grab according to browser if none defined
  if (!language) language = this.getBrowserLanguage();

  // inflexible: can't accept multiple languages
  // remove dialect/region code, leaving only the ISO 639 code
  // language = language.replace(/(-.*)+/, '');

  return language;
 },

 // build widget for changing the language cookie
 buildWidget: function (language) {

  this.jQuerycontainer = jQuery('<div/>');
  // link to language select description page
  this.jQuerycontainer.html('<a href="' + this.helpUrl + '" class="ls_link">' + this.stringHelpText + '</a> ');
  this.jQueryselect = jQuery('<select/>');


  var seen = {};
  jQuery('[lang]').each(function () {
   var lang = jQuery(this).attr('lang');
   if (!seen[lang]) {
    seen[lang] = true;
    multilingual.jQueryselect.append('<option>' + lang + '</option>');
   }
  });
  this.jQueryselect.prepend('<option value="showall">' + this.stringShowallText + '</option>');
  this.jQueryselect.attr('id', 'langselector');
  this.jQueryselect.val(this.getLanguage());
  this.jQueryselect.change(function () {
   multilingual.setCookieLanguage(jQuery('#langselector').val());
   multilingual.apply(jQuery('#langselector').val());
  });
  this.jQuerycontainer.append(this.jQueryselect);
  if (jQuery('#file').length) jQuery('#file').append(this.jQuerycontainer);
  else jQuery('#bodyContent').prepend(this.jQuerycontainer);
 },

 // main body of the function
 init: function () {
  if (typeof(ls_enable) != 'undefined') return;

  this.stringHelpText = (this.stringHelp[wgUserLanguage] || this.stringHelp['en']);
  this.stringShowallText = (this.stringShowAll[wgUserLanguage] || this.stringShowAll['en']);

  //disabling the gadget on special pages
  if (wgCanonicalNamespace == "Special") return;

  // only activated in view , purge, historysubmit or submit mode
  if (!((wgAction == 'view') || (wgAction == 'purge') || (wgAction == 'edit') || (wgAction == 'historysubmit') || (wgAction == 'submit'))) return;

  var collapsDesc = false;

  // grab an array of multilingual elements
  this.mls = jQuery('.multilingual');

  //Find {{en|...}} parent elements
  this.dls = jQuery('div.description[lang]').parent();

  //Only collaps if more than 4 descriptions
  this.dls.each(function () {
   if (jQuery(this).find('[lang]').length > 4 && jQuery(this).attr('id') != 'bodyContent' &&
      jQuery(this).attr('id') != 'wikiPreview' && jQuery(this).attr('id') != 'LangTableLangs') collapsDesc = true;
  });

  // Only build form if there are MLDs on page.
  if (!this.mls.length && !collapsDesc) return;

  if (collapsDesc) this.mls = this.dls.add('.multilingual');

  this.buildWidget();
  this.apply(this.getLanguage());
 },

 apply: function (language) {
  // if language is blank, delete the cookie and then recalculate
  if (!language) {
    this.deleteCookieLanguage();
    var language = this.getLanguage();
  }

  this.mls.each(function () {
   if (jQuery(this).parent('[class^="image_annotation_content"]').length) return true;

   // if (jQuery(this).parents('#template-picture-of-the-day').length) return true;

   if (jQuery(this).attr('id') == 'bodyContent' || jQuery(this).attr('id') == 'wikiPreview' || jQuery(this).attr('id') == 'LangTableLangs') return true;

   var jQueryrequestedLang = jQuery(this).find('[lang=' + language + ']');

   if (jQueryrequestedLang.length) {
     jQuery(this).children('[lang][lang!=' + language + ']').hide();
     jQuery(this).children('[lang=' + language + ']').show();
   } else {
     jQuery(this).children('[lang]').show();
   }
  });
 }
};

// register as onload function
jQuery(document).ready(function () {
  multilingual.init();
});

}); // Use cookie