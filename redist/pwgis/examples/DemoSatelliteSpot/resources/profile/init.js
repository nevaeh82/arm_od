Pw.Gis.UserSettings = {
    /**
     * Stylemap for displaying features
     */
    'featuresStyleMap' : new OpenLayers.StyleMap( {
        'default':{
            strokeColor: "#6F6FC8",
            strokeOpacity: 1,
            strokeWidth: 3,
            fillColor: "#9C9CC5",
            fillOpacity: 0.5,
            pointRadius: 6,
            "label": "",
            "labelXOffset": "${captionXOffset}",
            "labelYOffset": "${captionYOffset}",
            "fontColor": "${captionFontColor}",
            "fontSize": "${captionFontSize}",
            "fontFamily": "${captionFontFamily}",
            "fontWeight": "${captionFontWeight}",
            //halo settings - only in 2.12 version
            "labelOutlineColor": "${captionOutlineColor}",
            "labelOutlineWidth": "${captionOutlineWidth}"
        },
        'selected': {
            pointRadius: 12,
            strokeWidth: 6,
            fillColor: "#6F6FC8"
        }
    } ),

    "customStyles" : {
        "CoverageLowOrbital": {
            strokeColor: "#0F6FC8",
            strokeOpacity: 1,
            strokeWidth: 0,
            fillColor: "#0F6FC8",
            fillOpacity: 0.2,
            pointRadius: 6,
            layer: "CoverageLowOrbital",
            "pwAllowSelect" : true
        },
        "CoverageMediumOrbital": {
            strokeColor: "#e32636",
            strokeOpacity: 1,
            strokeWidth: 0,
            fillColor: "#e32636",
            fillOpacity: 0.2,
            pointRadius: 6,
            layer: "CoverageMediumOrbital",
            "pwAllowSelect" : true
        },
        "CoverageHighOrbital": {
            strokeColor: "#e3e126",
            strokeOpacity: 1,
            strokeWidth: 0,
            fillColor: "#e3e126",
            fillOpacity: 0.2,
            pointRadius: 6,
            layer: "CoverageHighOrbital",
            "pwAllowSelect" : true
        },
        "CoverageGeoStationary": {
            strokeColor: "#00ff00",
            strokeOpacity: 1,
            strokeWidth: 0,
            fillColor: "#00ff00",
            fillOpacity: 0.5,
            pointRadius: 6,
            layer: "CoverageGeoStationary",
            "pwAllowSelect" : true
        },
        "CoverageSatelliteDefined": {
            strokeColor: "#30d5c8",
            strokeOpacity: 1,
            strokeWidth: 0,
            fillColor: "#30d5c8",
            fillOpacity: 0.5,
            pointRadius: 6,
            layer: "CoverageSatelliteDefined",
            "pwAllowSelect" : true
        },
        "SearchRegion" : {
            strokeColor: "#30d5c8",
            strokeOpacity: 1,
            strokeWidth: 0,
            fillColor: "#30d5c8",
            fillOpacity: 0.5,
            pointRadius: 6,
            "pwAllowSelect" : true
        },
	"satpoint" :{
            pointRadius: 1,
            strokeColor: "#30d5c8",
            strokeOpacity: 1,
            strokeWidth: 1,
            fillColor: "#30d5c8",
            fillOpacity: 0.5,
            layer: "CoverageSatelliteDefined"
	}
        },
    enableDrawingTools : false,
    enableCurvimetrButton : false

};


window.initPolygonRingFunction = function() {
    client.getLinearRing =
    Pw.Gis.Client.getLinearRing = function( points ) {
        var geoPoints = [];
        for( var i=0; i < points.length; ++i ) {
            geoPoints.push( new OpenLayers.LonLat( points[i].lon, points[i].lat ) );
        }
        Pw.Gis.transformFromGeo( geoPoints );

        var mapPoints = [];
        for( var i=0; i < geoPoints.length; ++i ) {
            mapPoints.push( new OpenLayers.Geometry.Point( geoPoints[i].lon, geoPoints[i].lat ) );
        }
        return new OpenLayers.Geometry.LinearRing( mapPoints );
    }

    client.addPolygonRing =
    Pw.Gis.Client.addPolygonRing = function( id, pointsOuter, pointsInner, caption, tooltip, styles, featureType, indPart, countParts  )
    {
        var stringOuter = this.getLinearRing( pointsOuter );
        var stringInner= this.getLinearRing( pointsInner );

        if(pointsInner.length == 0) {
            var geometry = new OpenLayers.Geometry.Polygon( [stringOuter] );
        }
        else {
            var geometry = new OpenLayers.Geometry.Polygon( [stringOuter, stringInner] );
        }

        var allowableFeatureType = featureType;
        if ( allowableFeatureType == null ) {
            allowableFeatureType = Pw.Gis.FeatureType.Polygon;
        }

        var idPolygon = this.addFeature( id, geometry, caption, tooltip, styles, this._propType, allowableFeatureType, indPart, countParts );

        return idPolygon;
    };

if ( 0 ) {
	//enable for wrapDateLine

	var bounds = map.baseLayer.getMaxExtent().clone()
			.transform(map.baseLayer.projection, map.displayProjection);
	if (bounds.left - 0.001 <= -180 && bounds.right + 0.001 >= 180) {
		window.map.baseLayer.wrapDateLine = true;
	}
}
}


window.initLayerSwitcher = function() {
    map.baseLayer.displayInLayerSwitcher = false;
    layersManager.getLayerById("vectors").displayInLayerSwitcher = false;
    layersManager.getLayerById("markers").displayInLayerSwitcher = false;

    OpenLayers.Lang[OpenLayers.Lang.getCode()] = {};
    OpenLayers.Lang[OpenLayers.Lang.getCode()]['Overlays'] = "Отображаемые зоны:";

    map.getControlsByClass("OpenLayers.Control.LayerSwitcher")[0].draw();
}

