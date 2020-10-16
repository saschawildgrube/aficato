$this->IncludeElement("googlemaps");

$strApiKey = $this->GetWebsite()->GetConfig("googlemaps_apikey");
if ($strApiKey == "")
{
	$strOutput = '<p style="color:red">Google Maps api key is missing. Please add the api key to the config: "googlemaps_apikey"</p>';
}
else
{
	$this->IncludeElement("googlemaps");
	$googlemap = new CGoogleMapsElement($this->GetWebsite(),"default");
	$strOutput = $googlemap->RenderGoogleMaps(
		"Googleplex, 1600 Amphitheatre Pkwy, Mountain View, CA 94043");
}