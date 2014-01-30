/*
* StockPhoto - shows download and attribution buttons
* Original code 2010-09/10 by [[User:Magnus Manske]]
*/

// Disabled for Opera 9.27 and below
if (typeof StockPhoto == 'undefined' && wgNamespaceNumber == 6 && (wgAction == 'view' || wgAction == 'purge') && !$.browser.msie && (!$.browser.opera || ($.browser.opera && parseFloat($.browser.version) > 9.27))) {

	// Import CSS definitions
	importStylesheet( 'MediaWiki:Stockphoto.css' );

	var StockPhoto = {

		// Config
		rev: '32',
		show_social_bookmarks: false,

		// Misc
		ui_icon_download: 'http://upload.wikimedia.org/wikipedia/commons/thumb/9/92/Gnome-document-save.svg/50px-Gnome-document-save.svg.png',
		ui_icon_web: 'http://upload.wikimedia.org/wikipedia/commons/thumb/c/c0/Gnome-emblem-web.svg/50px-Gnome-emblem-web.svg.png',
		ui_icon_wiki: 'http://upload.wikimedia.org/wikipedia/commons/thumb/2/2c/Tango_style_Wikipedia_Icon.svg/50px-Tango_style_Wikipedia_Icon.svg.png',
		ui_icon_email: 'http://upload.wikimedia.org/wikipedia/commons/thumb/e/ee/Gnome-mail-send.svg/50px-Gnome-mail-send.svg.png',
		ui_icon_help: 'http://upload.wikimedia.org/wikipedia/commons/thumb/e/e7/Dialog-information_on.svg/50px-Dialog-information_on.svg.png',
		ui_icon_remove: 'http://upload.wikimedia.org/wikipedia/commons/thumb/9/9d/Emblem-unreadable.svg/20px-Emblem-unreadable.svg.png',
		information_template_hints: ['fileinfotpl_desc', 'fileinfotpl_src'],
		icons_only: ['midi', 'ogg', 'flac'],
		horizontal_ui: ['midi', 'ogg', 'flac', 'pdf', 'djvu'],
		//license_patterns: "^Artistic-2$", "^BSD images$", "^OS OpenData$", "^Mozilla Public License$"],
		ogg_icon: stylepath + '/common/images/icons/fileicon-ogg.png',
		stockphoto_code1: undefined,
		stockphoto_author: undefined,
		stockphoto_license: undefined,
		nicetitle: undefined,
		file_icon: undefined,
		file_url: undefined,
		backlink_url: undefined,
		attribution: '',
		fade_target: '',
		gfdl_note: false,
		sbm_counter: 1,
		fromCommons: false,
		attrRequired: true,

		init: function () {
			if (typeof stockphoto_prevent != "undefined") return;
			$("#filetoc").find("a[href=#file], a[href=#filehistory], a[href=#filelinks], a[href=#metadata], a[href=#globalusage]").parent().addClass("mw-filetoc-links");
			if ($.cookie('StockPhotoDisabled')) {
				var $enable = $('<li class="mw-filetoc-links"><a id="SpEnable">' + this.i18n.reuse + '</a></a>');
				$enable.click(function (){
					$.cookie('StockPhotoDisabled', null, {
						path: '/'
					});
					StockPhoto.init();
				});
				$("#filetoc").append($enable);
			  return;
			}
			if ($('#file').length < 1) return;
			this.small_horizontal_layout = false;
			if (wgUserName !== null) this.small_horizontal_layout = true;
			if (getParamValue('stockphoto_show_social_bookmarks') !== null) this.show_social_bookmarks = true;
			var has_information = false;
			$.each(this.information_template_hints, function (k, v) {
				if ($('#' + v).length !== 0) has_information = true;
			});

			// No {{Information}}
			if (!has_information) return;

			// Has one or more problemtags
			// Changed to also include renames and normal deletes
			if ($('.nuke').length) return;

			$('#stockphoto_base').remove();
			var img_width = $('#file img').width();
			var img_height = $('#file img').height();
			var xoff = img_width + 40;
			var yoff = $('#file').position().top + 5;
			if (!horizontal && img_height < 300) this.small_horizontal_layout = true;
			var horizontal = this.small_horizontal_layout;
			$.each(this.horizontal_ui, function (k, v) {
				v = new RegExp('\.' + v + '$', 'i');
				if (wgTitle.match(v)) horizontal = true;
			});
			if ($(window).width() < 1030) horizontal = true;

			// Initialize values
			this.share_this(-1);
			var html = "<div id='stockphoto_base' ";
			if (horizontal) html += "class='horizontal' >";
			else if (!$("body.rtl").length) html += "class='vertical' style='left:" + xoff + "px;top:" + yoff + "px'>";
			else html += "class='vertical' style='left: 0px;top:" + yoff + "px'>";

			html += this.add_button_row(this.ui_icon_download, "call_download", this.i18n.download, this.i18n.all_sizes, horizontal);
			html += this.add_button_row(this.ui_icon_web, "call_web", this.i18n.use_this_file_web_short, this.i18n.on_a_website, horizontal);
			html += this.add_button_row(this.ui_icon_wiki, "call_wiki", this.i18n.use_this_file_wiki_short, this.i18n.on_a_wiki, horizontal);
			html += this.add_button_row(this.ui_icon_email, "send_email", this.i18n.email_link_short, this.i18n.to_this_file, horizontal);
			html += this.add_button_row(this.ui_icon_help, "call_help", this.i18n.information, this.i18n.about_reusing, horizontal);
			html += '<a title="' + this.i18n.remove_icons + '" id="stockphoto_remove"><img src="' + this.ui_icon_remove + '" /></a>';
			if (this.show_social_bookmarks) html += this.add_social_icons(horizontal);
			html += "</div>";
			if ( this.small_horizontal_layout && $("#file").length ) {
				$(".mw-filetoc-links").hide();
				$("#filetoc").append(html);
			} else {
				$("#filetoc").after(html);
			}
			$('#stockphoto_remove').click(function () {
				$.cookie('StockPhotoDisabled', true, {
					expires: 60,
					path: '/'
				});
				$('#stockphoto_base').remove();
				$(".mw-filetoc-links").show();
			});

			$('#stockphoto_base img').parent().fadeTo(0, 0.7);
			$('#stockphoto_base>span').hover(function () {
				$(this).find('span:first a').fadeTo('fast', 1);
			}, function (evt) {
				$(this).find('span:first a').fadeTo('fast', 0.7);
			});

		},

		add_button_row: function (icon_url, fkt, txt, html, horizontal) {
			if (this.small_horizontal_layout) {
				icon_url = icon_url.replace('/50px-', '/20px-');
			}
			var imgid = "stockphoto_icon_" + fkt;
			var a = "<a href='#' title='" + txt + " " + html + "' onclick='StockPhoto." + fkt + "(); return false;'>";

			var ret = "<span id='stockphoto_" + fkt + "'>";
			ret += "<span class='stockphoto_buttonrow_icon'>" + a + "<img id='" + imgid + "' src='" + icon_url + "' /></a></span>";
			ret += "<span class='stockphoto_buttonrow_text'>" + a;
			if (this.small_horizontal_layout) ret += txt + "</a>";
			else ret += "<b>" + txt + "</b></a><br/>" + html;
			ret += "</span></span>";
			return ret;
		},

		stockphoto_get_thumbnail_url: function (width) {
			if (this.isset(this.file_icon)) return this.file_icon;
			var thumb_url;
			var alt_title = wgCanonicalNamespace + ":" + wgTitle;
			$('#file img').each(function () {
				if ($(this).attr('alt') != alt_title) return;
				thumb_url = $(this).attr('src').split('/');
			});
			// Special case of mwEmbed rewrite
			if( !thumb_url && $('#mwe_ogg_player_1').length ){
				return $('#mwe_ogg_player_1').find('img').attr('src');
			}
                        if( !thumb_url || thumb_url.length < 1 ) return;

			var last = thumb_url.pop().replace(/^\d+px-/, width + 'px-');
			thumb_url.push(last);
			thumb_url = thumb_url.join('/');
			return thumb_url;
		},

		is_audio_video_asset: function(url){
			var ext = url.substr(-3);
			if( ext == 'ogv' || ext == 'ogg' || ext == 'oga' ){
				return true;
			}
			return false;
		},

		make_html_textarea: function (e) {
			if(e) e.preventDefault(); // if()-check in case it wasn't fired by an event

			var width = $('#stockphoto_html_select').val();
			var type = $("input[name='stockphoto_code_type']:checked").val();

			// Iframe share for mwEmbed player
			if( this.is_audio_video_asset( this.backlink_url ) && type == 'html' ){
				// Get the ratio ( from html or from mwEmbed player ) 
				var height = ( $('#mwe_ogg_player_1').width() )?
						width * $('#mwe_ogg_player_1').height() / $('#mwe_ogg_player_1').width() :
						width * $('#file img,#file video').height() / $('#file img,#file video').width() ;
				// For audio that has zero height: 
				if( height == 0 ) height = 20;
				$('#stockphoto_html').text( '<iframe src="' + this.backlink_url + 
					'?withJS=MediaWiki:MwEmbed.js&embedplayer=yes" width="' + width + '" height="'+ height + '" frameborder="0" ></iframe>' );
				return ;
			}

			var thumb_url = this.stockphoto_get_thumbnail_url(width);

			var t;
			if (type == "html") t = "<a title='" + this.escapeAttribute(this.complete_attribution_text) + "' href='" + this.backlink_url + "'><img width='" + width + "' alt='" + this.escapeAttribute(this.nicetitle) + "' src='" + thumb_url + "'/></a>";
			else if (type == "bbcode") t = "[url=" + this.backlink_url + "][img]" + thumb_url + "[/img][/url]\n[url=" + this.backlink_url + "]" + this.nicetitle + "[/url]" + this.stockphoto_license + ", " + this.i18n.by + " " + this.stockphoto_author + ", " + this.i18n.from_wikimedia_commons;
			$('#stockphoto_html').text(t);
		},

		get_author_attribution: function (use_html) {

			var author = $.trim($("#fileinfotpl_aut + td").text());
			var source = $.trim($("#fileinfotpl_src + td").text());

			// Remove boiler template; not elegant, but...
			if (author.match(/This file is lacking author information/)) author = '';
			if (author.match(/^[Uu]nknown$/)) author = '';
			author = author.replace(/\s*\(talk\)$/i, "");

			if (author.indexOf('Original uploader was') != -1) {
				author = author.replace(/\s*Original uploader was\s*/g, "");
				this.fromCommons = true;
			}
			// Remove boiler template; not elegant, but...
			if (source.match(/This file is lacking source information/)) source = '';
			if (author !== '' && $('#own-work').length) { // Remove "own work" notice
				source = '';
				this.fromCommons = true;
			}
			if (author !== '' && source.length > 50) source = ''; // Remove long source info
			if (author.substr(0, 3) == "[&#9660;]") {
				author = author.substr(3);
				author = $.trim(author.split("Description").shift());
			}

			this.attribution = '';
			if (author !== '') this.attribution = author;
			if (source != '') {
				if (this.attribution != '') this.attribution += " (" + source + ")";
				else this.attribution = source;
			}
			this.stockphoto_author = this.attribution;
			if (author !== '') this.attribution = this.i18n.by_u + " " + this.attribution;
			else this.attribution = this.i18n.see_page_for_author;

			if ($('#creator').length) {
				this.attribution = $('#creator').text();
			}

			if ($('.licensetpl_aut').length) {
				if (use_html) this.attribution = $('.licensetpl_aut').eq(0).html();
				else this.attribution = $('.licensetpl_aut').eq(0).text();
			}

			if ($('.licensetpl_attr').length) {
				if (use_html) this.attribution = $('.licensetpl_attr').eq(0).html();
				else this.attribution = $('.licensetpl_attr').eq(0).text();
			}

			if ($("#fileinfotpl_credit + td").length) {
				if (use_html) this.attribution = $("#fileinfotpl_credit + td").html();
				else this.attribution = $("#fileinfotpl_credit + td").text();
			}

		},

		get_license: function (generate_html) {
			var licenses = new Array();
			$readable = $('.licensetpl');

			if (!$readable.length) {
				this.stockphoto_license = "[" + this.i18n.see_page_for_license + "]";
				return;
			}
			$readable.each(function () {
				var cL = {};

				cL['link'] = $(this).find('.licensetpl_link').html();
				cL['short'] = $(this).find('.licensetpl_short').html();
				cL['long'] = $(this).find('.licensetpl_long').html();
				cL['attr'] = $(this).find('.licensetpl_attr').html();
				cL['aut'] = $(this).find('.licensetpl_aut').html();
				cL['link_req'] = $(this).find('.licensetpl_link_req').html();
				cL['attr_req'] = $(this).find('.licensetpl_attr_req').html();

				if (cL['short'] !== null) licenses.push(cL);
			});

			if (licenses.length > 0) {
				$.each(licenses, function (k, v) {
					if (v['attr_req'] == "false") StockPhoto.attrRequired = false;
					if (v['short'].indexOf('GFDL') != -1) StockPhoto.gfdl_note = true;
					if (generate_html && v['link']) {
						licenses[k] = '<a href="' + v['link'] + '">' + v['short'] + '</a>';
					} else {
						if (v.link_req == "true") {
							licenses[k] = v['short'] + ' (' + v['link'] + ')';
						} else {
							licenses[k] = v['short'];
						}
					}
				});

				if (licenses.length > 1) {
					var l2 = licenses.pop();
					var l1 = licenses.pop();
					licenses.push(l1 + " " + this.i18n.or + " " + l2);
				}
				this.stockphoto_license = " [" + licenses.join(', ') + "]";
			} else {
				this.stockphoto_license = " [" + this.i18n.see_page_for_license + "]";
			}
		},

		get_attribution_text: function () {
			from = (this.fromCommons) ? this.i18n.from_wikimedia_commons : this.i18n.via_wikimedia_commons;
			html = ($("#stockphoto_attribution_html:checked").length) ? true : false;

			this.get_license(html);
			this.get_author_attribution(html);

			if ($("#fileinfotpl_credit + td").length) text = this.attribution;
			else text = this.attribution + this.stockphoto_license;

			if (html) text += ", <a href='" + this.escapeAttribute(this.backlink_url) + "'>" + from + "</a>";
			else text += ", " + from;

			return text;
		},

		refresh_attribution: function () {
			$("#stockphoto_attribution").val(StockPhoto.get_attribution_text());
		},

		createDialogRow: function (label, prefill, id) {
			idtext = (id) ? "id='" + id + "'" : ""
			return "<div class='stockphoto_dialog_row'><b>" + label + ":</b><br><input type='text' readonly " + idtext + " onClick=select() value='" + prefill + "'/></div>";
		},

		share_this: function (ui_mode) {
			this.complete_attribution_text = this.get_attribution_text();

			this.file_url = $("#file > a").attr("href");
			if (!this.file_url) this.file_url = $("#file > div > div > a").attr("href");
			if (!this.file_url) this.file_url = $("div.fullMedia a").attr("href");

			this.nicetitle = wgTitle.split('.');
			this.nicetitle.pop();
			this.nicetitle = this.nicetitle.join('.');

			$.each(this.icons_only, function (i, v) {
				var re = new RegExp('\.' + v + '$', 'i');
				if (!wgPageName.match(re)) return;
				StockPhoto.file_icon = StockPhoto.ogg_icon;
			});

			this.backlink_url = "http://commons.wikimedia.org/wiki/" + encodeURI(wgPageName);
			
			//Grab width in pixel from DOM, and trim it down
			//This does not yet work for SVGs or videos
			try {
				var pixelStr = $('.fullImageLink').contents().get(5).data;
				var widthSearchMatch = /([0-9 ,.\u00a0]+)\s*×/.exec(pixelStr);
				var imageWidth = parseInt(widthSearchMatch[1].replace(/[ ,.\u00a0]/g, ''));
				if (isNaN(imageWidth)) throw 'Cannot parse';

				//Calculate to which power of two we should go
				var power = Math.floor(Math.log(imageWidth) / Math.log(2));

				//Push 6 width to array
				var widths = [];
				for (var i=0; i < 5; i++) {
					widths.push(Math.pow(2, power-i));
				}
				widths = widths.reverse();
				
			} catch (e) {
				var widths = [75, 100, 120, 240, 500, 640, 800, 1024];
			}
			if (ui_mode == -1) return;

			var html = "";
			html += this.createDialogRow(this.i18n.page_url, this.escapeAttribute(this.backlink_url));
			html += this.createDialogRow(this.i18n.file_url, this.escapeAttribute(this.file_url));
			html += this.createDialogRow(this.i18n.attribution, this.escapeAttribute(this.complete_attribution_text), 'stockphoto_attribution');
			html += "<input id='stockphoto_attribution_html' onclick='StockPhoto.refresh_attribution()' type='checkbox' /><label for='stockphoto_attribution_html'>" + this.i18n.html + "</label>";
			if (this.gfdl_note) html += "<br/><span class='stockphoto_note'>" + this.i18n.gfdl_warning + "</span>";
			if (!this.attrRequired) html += "<br/><span class='stockphoto_note'>" + this.i18n.no_attr + "</span>";

			switch (ui_mode) {
			case 1:

				dtitle = this.i18n.download_this_file;
				if (this.isset(this.file_url)) {
					html += "<div><b>" + this.i18n.download_image_file + ":</b><br>";
					var dl_links = new Array();
					$.each(widths, function (i, v) {
						if (StockPhoto.isset(StockPhoto.file_icon)) return;
						dl_links.push("<a href='" + StockPhoto.stockphoto_get_thumbnail_url(v) + "'>" + v + "px</a>");
					});
					if (this.file_url) dl_links.push("<a href='" + this.file_url + "'>" + this.i18n.full_resolution + "</a>");
					if (dl_links.length) html += dl_links.join(" | ");
					else html += "<i>" + this.i18n.not_available + "</i>";
					html += "</div>";
				}


				break;

			case 2:
				dtitle = this.i18n.use_this_file_web;
				html += "<div class='stockphoto_dialog_row'><div style='float:right'>";
				html += "<input type='radio' name='stockphoto_code_type' value='html' id='stockphoto_code_type_html' onchange='StockPhoto.make_html_textarea();' checked /><label for='stockphoto_code_type_html'>" + StockPhoto.i18n.html + "</label> ";
				html += "<input type='radio' name='stockphoto_code_type' value='bbcode' id='stockphoto_code_type_bbcode' onchange='StockPhoto.make_html_textarea();' /><label for='stockphoto_code_type_bbcode'>" + StockPhoto.i18n.bbcode + "</label> ";

				html += '<select id="stockphoto_html_select" onchange="StockPhoto.make_html_textarea();">';
				var best_fit = 75;
				if (this.isset(this.file_icon)) {
					best_fit = 120;
					html += "<option value='120'>120" + this.i18n.px_wide_icon + "</option>";
				} else {
					$.each(widths, function (i, v) {
						if (v <= $('#file img').width()) best_fit = v;
						html += "<option value='" + v + "'>" + v + StockPhoto.i18n.px_wide + "</option>";
					});
				}
				html += '</select></div>';
				html += '<b>' + this.i18n.html + '/' + this.i18n.bbcode + ':</b><textarea onClick="select()" id="stockphoto_html" readonly="readonly" style="font-size:9pt">';
				html += '</textarea></div>';

				break;

			case 3:
				dtitle = this.i18n.use_this_file_wiki;

				html = this.createDialogRow(this.i18n.thumbnail, this.escapeAttribute("[[File:" + wgTitle + "|thumb|" + this.nicetitle + "]]"));
				html += this.createDialogRow(this.i18n.image, this.escapeAttribute("[[File:" + wgTitle + "|" + this.nicetitle + "]]"));

				break;
			}

			$("<div style='display:none' id='stockphoto_dialog'></div>").html(html).dialog({
				modal: true,
				width: 610,
				height: "auto",
				title: dtitle,
				close: function () {
					$(this).remove();
				}
			});
			$('#stockphoto_html_select').val(best_fit);

			this.make_html_textarea();
			$('#stockphoto_attribution_html').prev().css('width', '90%');
		},

		call_download: function () {
			StockPhoto.share_this(1);
		},

		call_web: function () {
			StockPhoto.share_this(2);
		},

		call_wiki: function () {
			StockPhoto.share_this(3);
		},

		call_help: function () {
			window.location.href = mw.util.wikiGetlink( StockPhoto.i18n.reusing_content_url );
		},

		send_email: function () {
			var url = "mailto:?subject=" + encodeURIComponent(StockPhoto.nicetitle) + "&body=" + encodeURIComponent(StockPhoto.backlink_url + "\n\n" + this.complete_attribution_text + " " + StockPhoto.i18n.from_wikimedia_commons);
			window.location.href = url;
		},

		escapeAttribute: function (s) {
			if (typeof s == "undefined") return "";
			return s.replace(/\n/g, ' ').replace(/\r/g, ' ').replace(/"/g, '&quot;').replace(/'/g, '&#039;');
		},

		isset: function (v) {
			return (typeof(v) != 'undefined');
		},

		i18n: {
			reuse: 'Reuse this file',
			download: 'Download',
			download_this_file: "Download this file",
			use_this_file_web: "Use this file on the web",
			use_this_file_web_short: "Use this file",
			use_this_file_wiki: "Use this file on a wiki",
			use_this_file_wiki_short: "Use this file",
			email_link_short: "Email a link",
			information: "Information",
			remove_icons: "Remove these icons",
			all_sizes: "all sizes",
			on_a_website: "on the web",
			on_a_wiki: "on a wiki",
			to_this_file: "to this file",
			about_reusing: "about reusing",
			look_what_i_found: "Look what I found on Wikimedia Commons : ",
			from_wikimedia_commons: "from Wikimedia Commons",
			via_wikimedia_commons: "via Wikimedia Commons",
			by: "by",
			by_u: "By",
			see_page_for_author: "See page for author",
			see_page_for_license: "see page for license",
			page_url: "Page URL",
			file_url: "File URL",
			attribution: "Attribution",
			no_attr: "Attribution not legally required",
			or: "or",
			gfdl_warning: "Using this file might require attaching a full copy of the <a href='http://en.wikipedia.org/wiki/GNU_Free_Documentation_License'>GFDL</a>",
			download_image_file: "Download image file",
			full_resolution: "Full resolution",
			not_available: "not available",
			share_this_file: "Share this file",
			html: "HTML",
			bbcode: "BBCode",
			px_wide_icon: "px wide (icon)",
			px_wide: "px wide",
			wikipedia_instant_commons: "Wikimedia/InstantCommons",
			thumbnail: "Thumbnail",
			image: "Image",
			share_on_facebook: "Bookmark with Facebook",
			share_on_digg: "Share on Digg.com",
			share_on_delicious: "Share on delicious",
			share_on_reddit: "Share on reddit.com",
			share_on_stumbleupon: "Share on stumbleupon.com",
			share_on_yahoo_buzz: "Share on Yahoo! Buzz",
			share_on_identi_ca: "Share on identi.ca",
			share_on_google_buzz: "Share on Google Buzz",
			share_on_twitter: "Share on twitter.com",
			reusing_content_url: "Commons:Reusing_content_outside_Wikimedia"
		}
	}

	if (wgUserLanguage != 'en') {
		importScript('MediaWiki:Stockphoto.js/' + wgUserLanguage);
	}
	mediaWiki.loader.using('jquery.ui.dialog', function() {
		$(document).ready(function () {
			StockPhoto.init();
		});
	});
}

// i18n on subpages [[MediaWiki:StockPhoto.js/langcode]]:
// StockPhoto.i18n = { ... }