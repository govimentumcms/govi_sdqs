(function($) {
  function cambiarTipoID(t) {
    if (t.val() == '2') {
      jQuery(t).closest('fieldset').find('.form-item-razon-social').css('display','block');
      jQuery(t).closest('fieldset').find('input').css('display','block')
      jQuery(t).closest('fieldset').find('.form-item-genero').parent().css('display','none');
      jQuery(t).closest('fieldset').find('.form-item-razon-social-poderante').css('display','block');

    } else {
      jQuery(t).closest('fieldset').find('.form-item-genero').parent().css('display','block');
      jQuery(t).closest('fieldset').find('.form-item ').css('display','block');
      jQuery(t).closest('fieldset').find('.form-item-razon-social').css('display','none');
      jQuery(t).closest('fieldset').find('.form-item-razon-social-poderante').css('display','none');

    }


  }
    function restaurarFormulario(){
        jQuery('fieldset.id-data').css('display','block');
        jQuery('fieldset.contact').css('display','block');
        jQuery('.form-item-razon-social-poderante').css('display','none');
        jQuery('.no-anon').css('display','block');
        jQuery('.contact legend').text("Ubicación y contacto");
        jQuery('.contact .email span.form-required').css('display','block');
        jQuery('.identificacion legend').text("Datos de identificación");
        jQuery('[name="tipo_identificacion"]').html('');
        window.sqsd_tipos_id.forEach(function(opt){
          jQuery('[name="tipo_identificacion"]').append('<option value="'+opt.id+'">'+opt.name+'</option>');
        });

    }
    function actualizarTipoIdentificacion(identificaciones_validas) {
      actual=window.sqsd_tipos_id.map(function (el) { return el.id; });
      opciones_eliminar_identificaciones = actual.filter(x => !identificaciones_validas.includes(x));
      opciones_eliminar_identificaciones.forEach(function(x){
        jQuery("[name='tipo_identificacion'] option[value='"+x+"']").remove();
      });
    }

    function cambiarTipoIdentificacion(t) {
        if (t.val() == 'juridica') {
            restaurarFormulario();
            actualizarTipoIdentificacion(Array('1','2'));
            jQuery('[name="tipo_identificacion"]').val('2');
            jQuery('[name="tipo_identificacion"]').val('2');

            jQuery('.form-item-genero').parent().css('display','none');
            jQuery('.form-item-razon-social').css('display','block');
            jQuery('.apoderado').css('display','none');

        }
        else if (t.val() == 'juridica_comercial') {
            restaurarFormulario();
            actualizarTipoIdentificacion(Array('2'));
            jQuery('[name="tipo_identificacion"]').val('2');
            jQuery('[name="tipo_identificacion"]').val('2');
            jQuery('.form-item-genero').parent().css('display','none');
            jQuery('.form-item-razon-social').css('display','block');
            jQuery('.apoderado').css('display','none');

        }
        else if (t.val() == 'apoderado') {
            restaurarFormulario();
            jQuery('.apoderado').css('display','block');
            jQuery('.identificacion legend').text("Datos de identificación del apoderado");
        }
        else if (t.val() == 'natural' ||  t.val() == 'apoderado') {
            jQuery('.form-item-nombre').css('display','block');
            jQuery('.form-item-apellido').css('display','block');
            jQuery('.form-item-genero').parent().css('display','block');
            jQuery('[name="tipo_identificacion"]').val('CC');
            jQuery('.form-item-razon-social').css('display','none');
            jQuery('.apoderado').css('display','none');

            restaurarFormulario();

        }
        else if (t.val() == 'infantil') {
            jQuery('[name="tipo_identificacion"]').val('5');
            jQuery('.form-item-nombre').css('display','block');
            jQuery('.form-item-apellido').css('display','block');
            jQuery('.form-item-genero').parent().css('display','block');
            jQuery('.form-item-razon-social').css('display','none');
            jQuery('.apoderado').css('display','none');

            restaurarFormulario();

        }
        else if (t.val() == 'anonimo') {
            jQuery('fieldset.id-data').css('display','none');
            jQuery('.no-anon').css('display','none');
            jQuery('.contact legend').text("Contacto (opcional)");
            jQuery('.contact .email span.form-required').css('display','block');
            jQuery('fieldset.contact').css('display','block');
            jQuery('.apoderado').css('display','none');


        }


    }



    Drupal.behaviors.sdqsForm = {
        attach: function(context, settings) {
            jQuery(document).ready(function() {
                window.sqsd_tipos_id = jQuery('[name="tipo_identificacion"] option').map(function() { return {id:jQuery(this).val(),name:jQuery(this).text()}; }).get();
                var selector_tipo_persona = jQuery('[name="tipo_peticionario"]');
                cambiarTipoIdentificacion(selector_tipo_persona);
                selector_tipo_persona.change(function() {
                    cambiarTipoIdentificacion(jQuery(this));
                });

                var selector_tipo_id = jQuery('[name="tipo_identificacion"]');
                selector_tipo_id.change(function() {
                    cambiarTipoID(jQuery(this));
                });
                var selector_tipo_id_poderante = jQuery('[name="tipo_identificacion_poderante"]');
                selector_tipo_id_poderante.change(function() {
                    cambiarTipoID(jQuery(this));
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
