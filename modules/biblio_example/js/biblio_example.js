(function($) {

// Controls the display of the "Abstract" field of a publication.
Drupal.behaviors.toggleAbstract = {
  attach: function() {
    $(".show-abstract").click(function() {

      var bid = $(this).attr('bid');

      if ($(this).hasClass('open')){
        $(this).removeClass('open');
      }
      else {
        $(this).addClass('open');
      }

      $("div .bid-" + bid).slideToggle("fast");
    });
  }
}
})(jQuery);
