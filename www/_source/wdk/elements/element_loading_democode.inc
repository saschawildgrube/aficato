$this->IncludeElement('loading');
$loading = new CLoadingElement($this->GetWebsite(),'default');
$strOutput = $loading->RenderLoading('Loading...');