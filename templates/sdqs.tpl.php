<?php //dsm(get_defined_vars()); 
$css = drupal_get_path('module', 'govi_sdqs') . '/assets/css/govi-sdqs.css';
drupal_add_css($css);
?>

<div id="servicio-ciudadano">
    <div id="sdqs">
	<div class="title"><h3><i class="logo"></i>Sistema Distrital de Quejas y Soluciones -SDQS-</h3></div>
	<div class="controles">
	    <div class="boton crear">
		<i></i>
		<a href="govi-sdqs/crear" class="colorbox-load colorbox-node" >Crear <span>Petición</span></a>
	    </div>
	    
	    <div class="boton consultar">
		<i></i>
		<a href="/govi-sdqs/consultar" class="colorbox-node">Consultar <span>Petición</span></a>
	    </div>
	    
	    <div class="info">
		<p>Si usted desea registrarse en el SDQS, <a href="http://sdqs.bogota.gov.co/sdqs/login" target="_blank">ingrese aquí</a></p>
	    </div>
	    <div class="select servicios">
		<?php echo views_embed_view('enlaces_servicio_ciudadania','block'); ?>
	    </div>
	</div>
    </div>
</div>
