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
