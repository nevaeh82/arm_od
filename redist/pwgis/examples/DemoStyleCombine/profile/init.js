function center(width)
{
    return - width/2;
}
function left(width)
{
    return 0;
}
function right(width)
{
    return -width;
}
function bottom(height)
{
    return -height;
}
function top(height)
{
    return 0;
}
function middle(height)
{
    return -height/2;
}

Pw.Gis.UserSettings = {
	"customStyles" : {
        mark: {
            externalGraphic: "images/greenmark48.png",
            graphicWidth: 124,
            graphicHeight: 124,
            graphicXOffset: center(124),
            graphicYOffset: bottom(124),
        },
        tooltip: {
            graphicTitle: "My cool mark"
        },
        labelColor: {
            fontColor: "red"
        },
        labelOutline: {
            labelOutlineWidth: 10,
            labelOutlineColor: "blue"
        },
        font :{
            fontFamily: "Courier",
            fontSize : "20pt",
        },
        behaviour: {
            pwAllowSelect : true,
            pwAllowDrag : true
        },
        line: {
            fillColor : "green",
        },
        lineOutline: {
            strokeColor: "yellow",
            strokeWidth : 20,
            strokeDashstyle: "longdashdot"
        }
    }
};
