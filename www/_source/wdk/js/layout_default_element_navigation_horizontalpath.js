// Layout Default Element Navigation Horizontalpath

$(window).resize(function () { 
   $('#navbar-fixed-top-overlap').css('padding-bottom', parseInt($('.navbar-fixed-top').css("height"))+10);
});
 
$(window).load(function () { 
   $('#navbar-fixed-top-overlap').css('padding-bottom', parseInt($('.navbar-fixed-top').css("height"))+10);
}); 