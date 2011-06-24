(function ($) {
  Drupal.behaviors.BiblioHighlight = {
    attach: function (context, settings) {
      $('a#biblio-highlight', context).click(function(e) {
        e.preventDefault();
        $("div.suspect").toggleClass('biblio-highlight');
      });
    }
  };
}(jQuery));