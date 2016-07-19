function includeJs(jsFilePath) {
    var js = document.createElement("script");

    js.type = "text/javascript";
    js.src = jsFilePath;

    document.body.appendChild(js);
}

includeJs(" /profiles/Zav/require.js ");

// Check ready to work with map fuctionality
SkyHobbitCheckReady = function() {
	if(typeof require == 'undefined') {
		return false;
	}
	
    if (typeof map == 'undefined') {
        return false;
    } else {
		SkyHobbitLoadRequire();
        return true;
    }
}

SkyHobbitLoadRequire = function() {
	require(["/profiles/Zav/main.js"], function(){ SkyHobbitInit(); });
	require(["/profiles/Zav/customActivateEdit.js"], function(){ SkyHobbitInit(); });
}

SkyHobbitInit = function() {
	Pw.Gis.Client.SkyHobbitSelection.init();
	Pw.Gis.Client.SkyHobbitAeroSelection.init();
	SkyHobbitEnableRightClick();
}