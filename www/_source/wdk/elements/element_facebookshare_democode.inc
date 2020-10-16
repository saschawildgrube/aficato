$this->IncludeElement("facebookshare");
$facebookshare = new CFacebookShareElement($this->GetWebsite(), "default");
$strOutput = $facebookshare->RenderFacebookShare();