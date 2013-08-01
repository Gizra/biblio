(function($) {

/**
* Controls the display of the "Abstract" field of a publication.
*/
Drupal.behaviors.toggleAbstract = {
  attach: function() {
    $(".show-abstract").click(function() {

      var bid = $(this).attr('bid');

      $(this).toggleClass('open');

      $(".abstract-body.bid-" + bid).slideToggle("fast");
    });
  }
}
})(jQuery);
