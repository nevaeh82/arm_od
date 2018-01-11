var profile = new Pw.Gis.ProfileManager();

var fileref = document.createElement("link")
fileref.setAttribute("rel", "stylesheet")
fileref.setAttribute("type", "text/css")
var cssPath = "/profiles/";
fileref.setAttribute("href", cssPath.concat(profile.getProfileName()).concat("/popupStyle.css"));

var ppopup = new OpenLayers.Popup("chicken", 
                              new OpenLayers.LonLat(5,40), 
                              new OpenLayers.Size(200,200), 
                              "example popup",  
                              true);  
                                       


if (typeof fileref != "undefined") {
    document.getElementsByTagName("head")[0].appendChild(fileref)
}

// =================================
var zoom = 15;
initPanel = function () {

    this.isInit = false;

    this.vpanel = new OpenLayers.Control.Panel({
        vertical: true
    });

    this.createButton = function (id) {
        if (toolpanel.getControlsByName("boardSearchButton").length == 0) {
            this.isInit = true;
            //Pw.Gis.Client.SkyHobbitSelection.init();
            map.addControl(this.vpanel);
            Pw.Gis.Client.SkyHobbitBlaPanel.vpanel.deactivate();
            var toolButton = new OpenLayers.Control.Button({
                'name': 'boardSearchButton',
                trigger: function () {
                    if (Pw.Gis.Client.SkyHobbitBlaPanel.vpanel.activate() == false) {
                        Pw.Gis.Client.SkyHobbitBlaPanel.vpanel.deactivate();
                    }
                },
                displayClass: "toolButtonBla" });
            toolpanel.addControls([toolButton]);
        }

        if(typeof Pw.Gis.Client.SkyHobbitBlaPanel.blaButtonArray[id] != 'undefined') {
            return;
        }

        var className = "blaPosButton";
        var addClassName = id;
        var displayName = id;
        id = id.concat("_popup");
        var resClass = className.concat(addClassName);

        var button = new OpenLayers.Control.Button({
            trigger: function () {
                if (Pw.Gis.Client.SkyHobbitBlaPopup.popupArray[id] != null) {
                    map.setCenter(Pw.Gis.Client.SkyHobbitBlaPopup.popupArray[id].lonlat, zoom);
                } else {
                    alert("No popup for this bla:(");
                }
                Pw.Gis.Client.SkyHobbitBlaPanel.vpanel.deactivate();
            },
            displayClass: resClass
        });


        //When reload map, controls may be stay in memory
        this.vpanel.controls.forEach( function(itm) {
            if( itm.displayClass == resClass ) {
                Pw.Gis.Client.SkyHobbitBlaPanel.removeButton(addClassName);
            }
        });
        // ----- fix -----

        this.vpanel.addControls([ button ]);
        this.blaButtonArray[id] = button;

        //========== Go to create style for BlaMenu item dynamically ========
        this.style =    '.' + resClass + 'ItemInactive:after,' +
                        '.' + resClass + 'ItemActive:after' + '{' +
                        'content: "БЛА №' + displayName + '";' +
                        'background-color: rgb(96,96,96); \
                        color: white; \
                        border-radius: 10; \
                        padding-left: 5px; \
                        padding-right: 5px; }';
        this.insertCss(this.style);

        this.style =    '.' + resClass + 'ItemInactive:before,' +
                        '.' + resClass + 'ItemActive:before' + '{' +
                        'content: url("/profiles/skyhobbit/plane_ic.png"); }';
        this.insertCss(this.style);
        //========== end style ========
    }

    this.removeButton = function (id) {
		id = id.concat("_popup");
        if (this.blaButtonArray[id] != null) {
            OpenLayers.Util.removeItem( this.vpanel.controls, this.blaButtonArray[id] );
			delete this.blaButtonArray[id];
            this.vpanel.redraw();
        }
    }

    this.insertCss = function (code) {
        var style = document.createElement('style');
        style.type = 'text/css';
        if (style.styleSheet) {
            // IE
            style.styleSheet.cssText = code;
        } else {
            // Other browsers
            style.innerHTML = code;
        }

        document.getElementsByTagName("head")[0].appendChild(style);
    }

    this.blaButtonArray = {};
}

//=================================================================================

function myShowFunction() {
	if(document.getElementById("panel").style.display == "block") {
		document.getElementById("panel").style.display = "none";
	} else {
		document.getElementById("panel").style.display = "block";
	}
}

SkyHobbitCustomPopup = function () {
    this.createPopup = function (id, x, y, data) {
        var displayId = id;
        id = id.concat("_popup");
        if (this.popupArray[id] == undefined) {
            point = new OpenLayers.LonLat(x, y);
            Pw.Gis.transformFromGeo([point]);
            var popup = new OpenLayers.Popup.FramedCloud(id,
                point,
                new OpenLayers.Size(0, 0),
                data,
                null,
                true);
            popup.imageSrc = "/profiles/Zav/cloud-popup-relative.png";
            popup.imageSize.h = 736;
            popup.imageSize.w = 1276;
            this.popupArray[id] = popup;
            map.addPopup(popup);
            this.hidePopup(id);

            Pw.Gis.Client.SkyHobbitBlaPanel.createButton(displayId);
        }
        else {
            point = new OpenLayers.LonLat(x, y);
            Pw.Gis.transformFromGeo([point]);
            this.popupArray[id].setContentHTML(data);
            this.popupArray[id].lonlat = point;
            this.popupArray[id].updatePosition();

            if( toolpanel.getControlsByName("boardSearchButton").length == 0) {
                 Pw.Gis.Client.SkyHobbitBlaPanel.createButton(displayId);
            }
        }
    };

    this.showPopup = function (id) {
        this.popupArray[id.concat("_popup")].show();
    };

    this.hidePopup = function (id) {
        this.popupArray[id].hide();
    };

    this.removePopup = function (id) {
		id = id.concat("_popup");
		if (this.popupArray[id] != null) {
			map.removePopup(this.popupArray[id]);
			delete this.popupArray[id];
		}
    }

    this.popupArray = {};
};

SkyHobbitAddBlaFeature = function(id, x, y, data) {
    Pw.Gis.Client.SkyHobbitBlaPopup.createPopup(id, x, y, data);
}

SkyHobbitRemoveBlaFeature = function(id) {
    Pw.Gis.Client.SkyHobbitBlaPanel.removeButton(id);
    Pw.Gis.Client.SkyHobbitBlaPopup.removePopup(id);
}

Pw.Gis.Client.SkyHobbitBlaPanel = new initPanel();
Pw.Gis.Client.SkyHobbitBlaPopup = new SkyHobbitCustomPopup();

//======================== Downloader feature. Custom Menu section ============================

SkyHobbitDownloadSelection = function () {
    this.drawControls = {};
    this.existFeature = {};
    var styleMap = new OpenLayers.StyleMap( { strokeDashstyle: "dash", fill: false, strokeColor: "#ff0000" } );
    this.boxLayer = new OpenLayers.Layer.Vector( "Область загрузки", {styleMap: styleMap} );

    this.init = function() {
	
        Pw.Gis.Client.SkyHobbitBlaPanel.isInit = false;
        map.addLayers([this.boxLayer]);

        this.drawControls = {
            box: new OpenLayers.Control.DrawFeature( this.boxLayer,
                OpenLayers.Handler.RegularPolygon, {
                    handlerOptions: {
                        sides: 4,
                        irregular: true
                    },
                    'displayClass': 'olControlDrawFeatureDownloadSelection',
                    'name': 'SelectionControl',
					'title': 'Загрузка по области',
                    featureAdded: function( feature ) {
                        Pw.Gis.Client.SkyHobbitAeroSelection.existFeature = feature;
                        PwGisHost.mapObjectEvents.featureClicked("", "downloadArea");
                        //Pw.Gis.Client.SkyHobbitSelection.drawControls.box.deactivate();
                    }
                } )
        }

        map.addControl(this.drawControls.box);
    };

    this.getGeometry = function() {
        toolpanel.activateControl(toolpanel.controls[0]); // unactivate control if success

		var feature = Pw.Gis.Client.SkyHobbitAeroSelection.existFeature;
        if ( feature == null ) {
            return [];
        }
        var points = feature.geometry.getVertices();

        var geoPoints = [];
        for( var i=0; i < points.length; ++i ) {
            var point = points[i];
            geoPoints.push( new OpenLayers.LonLat( point.x, point.y) );
        }
        Pw.Gis.transformToGeo( geoPoints );

        var param = [];
        for( var i=0; i < geoPoints.length; ++i) {
            var point = geoPoints[i];
            param.push( [point.lon, point.lat ]);
        }
        return param;
    };

    this.deinit = function() {
        this.boxLayer.removeAllFeatures();
		this.drawControls.box.deactivate();
    };
	
	this.cleanLayer = function() {
        this.boxLayer.removeAllFeatures();
    };
};

Pw.Gis.Client.SkyHobbitSelection = new SkyHobbitDownloadSelection();

SkyHobbitAeroSelection = function () {
	this.previousFeature = {};
    this.drawControls = {};
    this.existFeature = {};
	this.a = 1;
    var styleMap = new OpenLayers.StyleMap( { strokeDashstyle: "dash", fill: true, strokeColor: "#0000ff", fillColor: "#0000ff", fillOpacity: 0.3 } );
    this.boxLayer = new OpenLayers.Layer.Vector( "Область аэрофотосьемки", {styleMap: styleMap} );

    this.init = function() {

        Pw.Gis.Client.SkyHobbitBlaPanel.isInit = false;
        map.addLayers([this.boxLayer]);

        this.drawControls = {
            box: new OpenLayers.Control.DrawFeature( this.boxLayer,
                OpenLayers.Handler.Polygon, {
                    handlerOptions: {
                        sides: 4,
                        irregular: true
                    },
                    'displayClass': 'olControlDrawFeatureAeroSelection',
                    'name': 'SelectionAeroControl',
					'title': 'Аэрофотосьемка по области',
                    featureAdded: function( feature ) {
						var previousFeature = Pw.Gis.Client.SkyHobbitAeroSelection.existFeature;
                        Pw.Gis.Client.SkyHobbitAeroSelection.existFeature = feature;
						Pw.Gis.Client.SkyHobbitAeroSelection.boxLayer.removeFeatures([previousFeature]);
                        PwGisHost.mapObjectEvents.featureClicked("", "aeroArea");
                    }
                } )
        }

        map.addControl(this.drawControls.box);
    };
	
	this.setCustomPoly = function(id) {
		var customFeature = map.getLayer("vectors").getFeatureById(id).clone();
		map.getLayer("vectors").removeFeatures([map.getLayer("vectors").getFeatureById(id)]);
		customFeature.style = Pw.Gis.Client.SkyHobbitAeroSelection.boxLayer.styleMap.styles.default.defaultStyle;
		
		var previousFeature = Pw.Gis.Client.SkyHobbitAeroSelection.existFeature;
		Pw.Gis.Client.SkyHobbitAeroSelection.boxLayer.removeFeatures([previousFeature]);
		Pw.Gis.Client.SkyHobbitAeroSelection.boxLayer.addFeatures([customFeature]);
        Pw.Gis.Client.SkyHobbitAeroSelection.existFeature = customFeature;
	};

    this.getGeometry = function() {
        toolpanel.activateControl(toolpanel.controls[0]); // unactivate control if success

        //var feature= Pw.Gis.Client.SkyHobbitAeroSelection.boxLayer.features[0];
		var feature = Pw.Gis.Client.SkyHobbitAeroSelection.existFeature;
        if ( feature == null ) {
            return [];
        }
        var points = feature.geometry.getVertices();

        var geoPoints = [];
        for( var i=0; i < points.length; ++i ) {
            var point = points[i];
            geoPoints.push( new OpenLayers.LonLat( point.x, point.y) );
        }
        Pw.Gis.transformToGeo( geoPoints );

        var param = [];
        for( var i=0; i < geoPoints.length; ++i) {
            var point = geoPoints[i];
            param.push( [point.lon, point.lat ]);
        }
        return param;
    };

    this.deinit = function() {
        //this.boxLayer.removeAllFeatures();
		this.drawControls.box.deactivate();
    };
	
	this.cleanLayer = function() {
        this.boxLayer.removeAllFeatures();
    };
};

Pw.Gis.Client.SkyHobbitAeroSelection = new SkyHobbitAeroSelection();

//===========================================================================================

// Here we enabling right click on map. 
// A control class for capturing click events...
SkyHobbitEnableRightClick = function() {
	OpenLayers.Control.Click = OpenLayers.Class(OpenLayers.Control, {                

	defaultHandlerOptions: {
		'single': true,
		'double': true,
		'pixelTolerance': 0,
		'stopSingle': false,
		'stopDouble': false
	},
	handleRightClicks:true,
	initialize: function(options) {
		
		this.handlerOptions = OpenLayers.Util.extend(
			{}, this.defaultHandlerOptions
		);
		
		OpenLayers.Control.prototype.initialize.apply(
			this, arguments
		); 
		
		this.handler = new OpenLayers.Handler.Click(
			this, this.eventMethods, this.handlerOptions
		);
	},
	CLASS_NAME: "OpenLayers.Control.Click"
	});


	// Add an instance of the Click control that listens to various click events:
	var oClick = new OpenLayers.Control.Click({eventMethods:{
	 'rightclick': function(e) {
			var fId = e.toElement._featureId;
			if(!fId) {
				fId = "Undefined_Feature";
			}
			PwGisHost.mapObjectEvents.featureClicked(fId, "rightClick");
	  }
	 }
	});
	map.addControl(oClick);
	oClick.activate();
}
