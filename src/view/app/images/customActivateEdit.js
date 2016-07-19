SkyHobbitToolActivate = function(event) {
	var arguments = [];
	arguments.push("editToolActivate");
	arguments.push(true);
	PwGisHost.mapEvents.userEvent(arguments);
	
	// Bycicle fix for style. Custom function of active event drop default OL styling
	var k = document.getElementsByClassName("olControlEditFeatureItemInactive olButton");
	k[0].style.backgroundColor = "#fffe89";
	k[0].style.opacity = "0.8";
}

SkyHobbitToolDeactivate = function(event) {
	var arguments = [];
	arguments.push("editToolActivate");
	arguments.push(false);
	PwGisHost.mapEvents.userEvent(arguments);
	
	// Bycicle fix for style. Custom function of active event drop default OL styling
	var k = document.getElementsByClassName("olControlEditFeatureItemInactive olButton");
	k[0].style.backgroundColor = "white";
	k[0].style.opacity = "1";
}

toolpanel.controls[7].events.listeners.activate[0].func = SkyHobbitToolActivate;
toolpanel.controls[7].events.listeners.deactivate[0].func = SkyHobbitToolDeactivate;