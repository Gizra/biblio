(function($) {

  Drupal.behaviors.toggleAbstract = {
    attach: function() {
      $(".show-abstract").click(function() {

        var bid = $(this).attr('bid');

        $("div .bid_" + bid).slideToggle("fast");
      });
    }
  }
})(jQuery);
