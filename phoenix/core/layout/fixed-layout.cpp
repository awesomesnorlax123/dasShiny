void FixedLayout::append(Sizable& sizable, Geometry geometry) {
  children.append({&sizable, geometry});
  synchronizeLayout();
  if(window()) window()->synchronizeLayout();
}

void FixedLayout::append(Sizable& sizable) {
  for(auto& child : children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
  if(window()) window()->synchronizeLayout();
}

bool FixedLayout::enabled() {
  if(layout()) return state.enabled && layout()->enabled();
  return state.enabled;
}

Size FixedLayout::minimumSize() {
  unsigned width = MinimumSize, height = MinimumSize;
  for(auto& child : children) {
    width  = max(width,  child.sizable->minimumSize().width);
    height = max(height, child.sizable->minimumSize().height);
  }
  return {width, height};
}

void FixedLayout::remove(Sizable& sizable) {
  for(unsigned n = 0; n < children.size(); n++) {
    if(children[n].sizable == &sizable) {
      children.remove(n);
      Layout::remove(sizable);
      if(window()) window()->synchronizeLayout();
      break;
    }
  }
}

void FixedLayout::reset() {
  for(auto& child : children) {
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void FixedLayout::setEnabled(bool enabled) {
  state.enabled = enabled;
  for(auto& child : children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->enabled() : enabled);
  }
}

void FixedLayout::setGeometry(Geometry geometry) {
}

void FixedLayout::setVisible(bool visible) {
  state.visible = visible;
  for(auto& child : children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

void FixedLayout::synchronizeLayout() {
  for(auto& child : children) {
    Layout::append(*child.sizable);
    child.sizable->setGeometry(child.geometry);
  }
}

bool FixedLayout::visible() {
  if(layout()) return state.visible && layout()->visible();
  return state.visible;
}

FixedLayout::FixedLayout() {
  state.enabled = true;
  state.visible = true;
}

FixedLayout::~FixedLayout() {
  while(children.size()) remove(*children[0].sizable);
}
