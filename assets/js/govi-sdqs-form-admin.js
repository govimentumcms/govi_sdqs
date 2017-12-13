(function($) {
    
    Drupal.behaviors.sdqsForm = {
        attach: function(context, settings) {
            jQuery(document).ready(function() {
                jQuery('.hide').parent().hide();

                jQuery('.change-item').click(function(event){
                    event.preventDefault();
                    jQuery('.'+jQuery(this).data('change')).show();
                });
            });
        }
    };


}(jQuery));