$this->IncludeElement("carousel");
		
$carousel = new CCarouselElement($this->GetWebsite(),"default");
			
$arraySlides = array();
for ($nIndex = 1; $nIndex <= 3; $nIndex++)
{
	$arraySlide = array();
	$arraySlide["title"] = "Title #".$nIndex;
	$arraySlide["text"] = "Some text on slide #".$nIndex;
	$arraySlide["imageurl"] = HtmlEncode($this->GetWebsite()->GetLayoutImageURL("example"));
	$arraySlide["linktext"] = "Link #".$nIndex;
	$arraySlide["linkurl"] = "#slide1";
	$arraySlide["external"] = false;
	$arraySlides[] = $arraySlide;
}
$arraySlides[] = array( "payload" => "<h1>Some HTML code...</h1><p>...directly embedded on the slide</p>"); 

$strOutput = $carousel->RenderCarousel($arraySlides);