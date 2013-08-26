(function ($) {

  Drupal.behaviors.biblioUiFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.biblio-form-owner', context).drupalSetSummary(function (context) {
        var name = $('.form-item-name input', context).val();

        return Drupal.t('By @name', { '@name': name });
      });
    }
  };

})(jQuery);

(function ($) {

  Drupal.behaviors.biblioContributorUiFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.biblio-form-owner', context).drupalSetSummary(function (context) {
        var name = $('.form-item-publisher input', context).val();

        return Drupal.t('By @name', { '@name': name });
      });
    }
  };

})(jQuery);
