
<?php
$temas = variable_get('govi_sdqs_lista_tema');
$entidades = variable_get('govi_sdqs_lista_entidades');
$tipos_peticion = variable_get('govi_sdqs_lista_tipo_peticion');
$tipos_id = variable_get('govi_sdqs_lista_tipos_id');

if (empty($temas)
	|| empty($entidades)
	|| empty($tipos_peticion)
	|| empty($tipos_id)) {

        $msg = '<p>Uno o varios códigos del SDQS no se han cargado en el sistema. <br/>';
        $msg .= 'Ingrese a la configuración del Webservice del módulo ';
        $msg .= '<a href="/admin/config/features/sdqs-client" title="Configurar SDQS">govi_sdqs</a> para ';
        $msg .= 'configurar las descripciones y obtener los datos de los selectores de ';
        $msg .= 'este formulario.';

	drupal_set_message($msg, 'warning');
}

?>
<fieldset>
    <legend>Datos personales</legend>
    <div class="pure-g"> 
	<div class="pure-u-1 pure-u-md-1-2">
	    <?php print render($form['datos_personales']['nombre']); ?>
	</div>
	<div class="pure-u-1 pure-u-md-1-2">
	    <?php print render($form['datos_personales']['apellido']); ?>
	</div>
	<div class="pure-u-1 pure-u-md-1-2">
	    <?php print render($form['datos_personales']['tipo_identificacion']); ?>
	</div>
	<div class="pure-u-1 pure-u-md-1-2">
	    <?php print render($form['datos_personales']['numero_identificacion']); ?>
	</div>
    </div>
    <div class="pure-g">
	<div class="pure-u-1 pure-u-md-1-2">
	    <?php print render($form['datos_personales']['correo_electronico']); ?>
	</div>
    </div>
</fieldset>
<fieldset>
    <legend>Crear Petición</legend>
    <div class="pure-g">
	<div class="pure-1-1 pure-u-md-1-2">
	    <?php print render($form['pqr_crear']['tipo_peticion']); ?>
	</div>
	<div class="pure-1-1 pure-u-md-1-2">
	    <?php print render($form['pqr_crear']['tema']); ?>
	</div>
	<div class="pure-1-1 pure-u-md-1-2">
	    <?php print render($form['pqr_crear']['entidad']); ?>
	</div>
	<div class="pure-1-1 pure-u-md-23-24">
	    <?php print render($form['pqr_crear']['asunto']); ?>
	</div>
    </div>
</fieldset>
<div class="pure-g">
    <div class="pure-1-1 pure-u-md-1-1">
	<?php print render($form['submit']); ?>
    </div>
</div>

<?php print render($form['form_build_id']); ?>
<?php print render($form['form_token']); ?>
<?php print render($form['form_id']); ?>

<?php drupal_render_children($form); ?>
