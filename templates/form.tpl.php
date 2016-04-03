<?php 
if (variable_get('sdqs_env') === 'test') {
	$msg = 'Bienvenido al Sistema Distrital de Quejas y Soluciones -SDQS- <br/>';
	$msg .= '<hr/>';
	$msg .= 'Este módulo está temporalmente en <b>modo de pruebas</b>, ';
	$msg .= 'las peticiones enviadas a través de este formulario ';
	$msg .= '<b>no</b> serán atendidas por la Dirección Distrital de ';
	$msg .= 'Servicio al Ciudadano o entidad competente. ';
	$msg .= 'Si tiene alguna inquietud por favor escriba al correo electrónico: ';
	$msg .= '<a href="mailto:'.variable_get('site_mail').'" title="escribir">';
	$msg .= variable_get('site_mail').'</a> para obtener mayor información.<br>'; 
	$msg .= 'Si desea enviar una petición formal puede acceder al Sistema ';
	$msg .= 'Distrital de Quejas y Soluciones SDQS haciendo click ';
	$msg .= '<a href="http://www.bogota.gov.co/sdqs" title="Ir al Sistema Distrital de Quejas y Soluciones" target="_blank">';
	$msg .= 'aquí</a>.';

	drupal_set_message($msg, 'warning');
}

?>
<fieldset>
    <legend>Datos personales</legend>
    <div class="pure-g"> 
	<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['datos_personales']['nombre']); ?>
	</div>
	<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['datos_personales']['apellido']); ?>
	</div>
	<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['datos_personales']['tipo_identificacion']); ?>
	</div>
	<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['datos_personales']['numero_identificacion']); ?>
	</div>
    </div>
    <div class="pure-g">
	<div class="pure-u-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['datos_personales']['correo_electronico']); ?>
	</div>
    </div>
</fieldset>
<fieldset>
    <legend>Crear Petición</legend>
    <div class="pure-g">
	<div class="pure-1-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['pqr_crear']['tipo_peticion']); ?>
	</div>
	<div class="pure-1-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['pqr_crear']['tema']); ?>
	</div>
	<div class="pure-1-1 pure-u-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	    <?php print render($form['pqr_crear']['entidad']); ?>
	</div>
	<div class="pure-1-1 pure-u-sm-1 pure-u-md-23-24 pure-u-lg-23-24 pure-u-xl-23-24">
	    <?php print render($form['pqr_crear']['asunto']); ?>
	</div>
    <div class="pure-1-1 pure-u-sm-1 pure-u-md-23-24 pure-u-lg-23-24 pure-u-xl-23-24">

	    <?php print render($form['pqr_crear']['archivo']); ?>
	</div>
    </div>
</fieldset>
<div class="pure-g">
    <div class="pure-u-1 pure-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	<?php print render($form['captcha']); ?>
    </div>
    <div class="pure-u-1 pure-sm-1 pure-u-md-1-2 pure-u-lg-1-2 pure-u-xl-1-2">
	<?php print render($form['submit']); ?>
    </div>
</div>

<?php print render($form['form_build_id']); ?>
<?php print render($form['form_token']); ?>
<?php print render($form['form_id']); ?>

<?php drupal_render_children($form); ?>
