Pw.Gis.MapManager.registerExternalMap( 'mapnik', 'OSM MapINK', function( map ) {
	var mapnik = new OpenLayers.Layer.OSM('OSM mapnik');
	map.addLayer(mapnik);
});

Pw.Gis.MapManager.registerExternalMap( 'simpleTms', 'Simple TMS', function( map ) {
	var tms = new OpenLayers.Layer.TMS(
		//Имя для контролла выбора слоев
		"My TMS Layer",

		//путь до TMS сервера
		"http://tilecache.osgeo.org/wms-c/Basic.py/",

		//параметры специфичные для TMS: 
		//набор слоев и тип картинки, которую вы желаете получить (png/jpeg/gif) 
		{layername: "basic", type: "png"}
	);
	map.addLayer(tms);
});
