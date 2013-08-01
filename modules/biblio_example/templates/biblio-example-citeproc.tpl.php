<div class="single-publication clearfix">
  <div class="first">
    <div class="publication-image">
      <?php print $image; ?>
    </div>
  </div>
  <div class="second">
    <span class="publication-citation"><?php print $citation; ?></span>
    <span class="publication-abstract"><a class="show-abstract" bid="<?php print $id; ?>">Abstract</a></span>
    <span class="pdf-wrapper"><?php print $pdf_list; ?></span>
    <div class="abstract-body bid-<?php print $id; ?>"><?php print $abstract; ?></div>
  </div>
</div>