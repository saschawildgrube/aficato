$this->IncludeElement('icon');

$icon = new CIconElement($this->GetWebsite(), 'default');

$strOutput = '';
$strOutput .= $icon->RenderIcon('help');
$strOutput .= $icon->RenderIcon('mail');
$strOutput .= $icon->RenderIcon('link');
$strOutput .= $icon->RenderIcon('link-external');
$strOutput .= $icon->RenderIcon('list');
$strOutput .= $icon->RenderIcon('list-details');
$strOutput .= $icon->RenderIcon('back');
$strOutput .= $icon->RenderIcon('next');
$strOutput .= $icon->RenderIcon('search');
$strOutput .= $icon->RenderIcon('view');
$strOutput .= $icon->RenderIcon('modify');
$strOutput .= $icon->RenderIcon('duplicate');
$strOutput .= $icon->RenderIcon('delete');
$strOutput .= $icon->RenderIcon('undo');
$strOutput .= $icon->RenderIcon('import');
$strOutput .= $icon->RenderIcon('export');
$strOutput .= $icon->RenderIcon('execute');