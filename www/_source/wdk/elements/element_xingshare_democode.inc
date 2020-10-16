$this->IncludeElement("xingshare");
$xingshare = new CXingShareElement($this->GetWebsite(), "default");
$strOutput = $xingshare->RenderXingShare();