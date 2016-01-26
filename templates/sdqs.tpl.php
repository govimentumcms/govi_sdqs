<?php //dsm(get_defined_vars()); 
$css = drupal_get_path('module', 'govi_sdqs') . '/assets/css/govi-sdqs.css';
drupal_add_css($css);
?>

<div id="servicio-ciudadano">
    <div id="sdqs">
	<div class="title"><h3><i class="logo"></i>Sistema Distrital de Quejas y Soluciones</h3></div>
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
		<p>Si usted quiere registrarse en el Sistema Distrital de Quejas y Soluciones, ingrese <a href="#">AQUI</a></p>
	    </div>
	    <div class="select servicios">
		Más Servicios
	    </div>
	</div>
    </div>
</div>
