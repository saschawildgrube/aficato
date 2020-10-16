$this->IncludeElement("linkedinshare");
$linkedinshare = new CLinkedInShareElement($this->GetWebsite(), "default");
$strOutput = $linkedinshare->RenderLinkedInShare();