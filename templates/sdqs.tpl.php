<?php //dsm(get_defined_vars()); 
$path = drupal_get_path('module', 'govi_sdqs');
$css = $path . '/assets/css/govi-sdqs.css';
$js = $path . '/assets/js/govi-sdqs.js';
drupal_add_css($css);
drupal_add_js($js);

$tema = variable_get('govi_sdqs_tema') === 'light' ? 'class="light"' : ''; 
?>

<div id="servicio-ciudadano">
    <div id="sdqs" <?php print $tema; ?> >
	<div class="title"><h3>Sistema Distrital de Quejas y Soluciones -SDQS-</h3></div>
	<div class="controles">
	    <div class="boton crear">
		<a href="/govi-sdqs/crear" >Crear <span>Petición</span></a>
	    </div>
	    
	    <div class="boton consultar">
		<a href="http://sdqs.bogota.gov.co/sdqs/login" target="_blank" >Consultar <span>Petición</span></a>
	    </div>
	    
	    <div class="info">
		<p>Si usted desea registrarse en el SDQS, <a href="http://sdqs.bogota.gov.co/sdqs/login" target="_blank">ingrese aquí</a></p>
	    </div>
	    <div class="select servicios">
		<?php print views_embed_view('enlaces_servicio_ciudadania','block'); ?>
	    </div>
	</div>
    </div>
</div>
