#ifndef NT_WIDGET_ROOT_H
#define NT_WIDGET_ROOT_H

#include "w/group.h"

#include "w/page_help.h"
#include "w/page_seq.h"
#include "w/page_pat.h"
#include "w/page_ins.h"

namespace w {

struct Root : public Group {
  PageHelp help_;
  PageSeq seq_;
  PagePat pat_;
  PageIns ins_;
  Root(Scr *scr, AppState *state, ActionDispatcher dispatch);
  void draw();
  bool onKeyDown(int key, int scancode, int mods);
};

} // namespace w

#endif
