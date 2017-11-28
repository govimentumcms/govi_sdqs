(function($) {
    function restaurarFormulario(){
        jQuery('fieldset.id-data').css('display','block');
        jQuery('fieldset.contact').css('display','block');

        jQuery('.no-anon').css('display','block');
        jQuery('.contact legend').text("Ubicación y contacto");
        jQuery('.contact .email span.form-required').css('display','block');
    }

    function cambiarTipoIdentificacion(t) {
        if (t.val() == 'juridica') {
            jQuery('[name="tipo_identificacion"]').val('NI');
            jQuery('.form-item-razon-social').css('display','block');
            restaurarFormulario();
        } 
        else if (t.val() == 'natural' ||  t.val() == 'apoderado') {
            jQuery('[name="tipo_identificacion"]').val('CC');
            jQuery('.form-item-razon-social').css('display','none');
            restaurarFormulario();

        }
        else if (t.val() == 'infantil') {
            jQuery('[name="tipo_identificacion"]').val('RC');
            jQuery('.form-item-razon-social').css('display','none');
            restaurarFormulario();

        }
        else if (t.val() == 'anonimo') {
            jQuery('fieldset.id-data').css('display','none');
            jQuery('.no-anon').css('display','none');
            jQuery('.contact legend').text("Contacto (opcional)");
            jQuery('.contact .email span.form-required').css('display','none'); 
            jQuery('fieldset.contact').css('display','none');

        } else {
            restaurarFormulario();


        }
    }

    Drupal.behaviors.sdqsForm = {
        attach: function(context, settings) {
            jQuery(document).ready(function() {
                var selector_tipo_persona = jQuery('[name="tipo_solicitante"]');
                cambiarTipoIdentificacion(selector_tipo_persona);
                selector_tipo_persona.change(function() {
                    cambiarTipoIdentificacion(jQuery(this));
                });

                var selector_pais = jQuery('[name="pais"]');
                selector_pais.change(function() {
                    if (jQuery(this).val() != 42) {
                        jQuery('#wrapper-ciudades').parent().css('display', 'none');
                        jQuery('.form-item-departamento').parent().css('display', 'none');
                    } else {
                        jQuery('#wrapper-ciudades').parent().css('display', 'block');
                        jQuery('.form-item-departamento').parent().css('display', 'block');
                    }
                });
            });
        }
    };


}(jQuery));