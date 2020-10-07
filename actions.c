enum { WIN_NW, WIN_N, WIN_NE, WIN_W, WIN_C, WIN_E, WIN_SW, WIN_S, WIN_SE }; /* coordinates for moveplace */
void
moveplace(const Arg *arg)
{
	Client *c;
	int nh, nw, nx, ny;
	c = selmon->sel;
	if (!c || (arg->ui >= 9))
		 return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		togglefloating(NULL);
	nh = (selmon->wh / 3) - (c->bw * 2);
	nw = (selmon->ww / 3) - (c->bw * 2);
	nx = (arg->ui % 3) -1;
	ny = (arg->ui / 3) -1;
	if (nx < 0)
		nx = selmon->wx;
	else if(nx > 0)
		nx = selmon->wx + selmon->ww - nw - c->bw*2;
	else
		nx = selmon->wx + selmon->ww/2 - nw/2 - c->bw;
	if (ny <0)
		ny = selmon->wy;
	else if(ny > 0)
		ny = selmon->wy + selmon->wh - nh - c->bw*2;
	else
		ny = selmon->wy + selmon->wh/2 - nh/2 - c->bw;
	resize(c, nx, ny, nw, nh, True);
	XWarpPointer(dpy, None, c->win, 0, 0, 0, 0, nw/2, nh/2);
}

void
movestack(const Arg *arg) {
	Client *c = NULL, *p = NULL, *pc = NULL, *i;

	if(arg->i > 0) {
		/* find the client after selmon->sel */
		for(c = selmon->sel->next; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);
		if(!c)
			for(c = selmon->clients; c && (!ISVISIBLE(c) || c->isfloating); c = c->next);

	}
	else {
		/* find the client before selmon->sel */
		for(i = selmon->clients; i != selmon->sel; i = i->next)
			if(ISVISIBLE(i) && !i->isfloating)
				c = i;
		if(!c)
			for(; i; i = i->next)
				if(ISVISIBLE(i) && !i->isfloating)
					c = i;
	}
	/* find the client before selmon->sel and c */
	for(i = selmon->clients; i && (!p || !pc); i = i->next) {
		if(i->next == selmon->sel)
			p = i;
		if(i->next == c)
			pc = i;
	}

	/* swap c and selmon->sel selmon->clients in the selmon->clients list */
	if(c && c != selmon->sel) {
		Client *temp = selmon->sel->next==c?selmon->sel:selmon->sel->next;
		selmon->sel->next = c->next==selmon->sel?c:c->next;
		c->next = temp;

		if(p && p != c)
			p->next = c;
		if(pc && pc != selmon->sel)
			pc->next = selmon->sel;

		if(selmon->sel == selmon->clients)
			selmon->clients = c;
		else if(c == selmon->clients)
			selmon->clients = selmon->sel;

		arrange(selmon);
	}
}

void
switchcol(const Arg *arg)
{
	Client *c, *t;
	int col = 0;
	int i;

	if (!selmon->sel)
		return;
	for (i = 0, c = nexttiled(selmon->clients); c ;
	     c = nexttiled(c->next), i++) {
		if (c == selmon->sel)
			col = (i + 1) > selmon->nmaster;
	}
	if (i <= selmon->nmaster)
		return;
	for (c = selmon->stack; c; c = c->snext) {
		if (!ISVISIBLE(c))
			continue;
		for (i = 0, t = nexttiled(selmon->clients); t && t != c;
		     t = nexttiled(t->next), i++);
		if (t && (i + 1 > selmon->nmaster) != col) {
			focus(c);
			restack(selmon);
			break;
		}
	}
}

/* Selects for the view of the focused window. The list of tags */
/* to be displayed is matched to the focused window tag list. */
void
winview(const Arg *arg){
	Window win, win_r, win_p, *win_c;
	unsigned nc;
	int unused;
	Client* c;
	Arg a;

	if (!XGetInputFocus(dpy, &win, &unused)) return;
	while(XQueryTree(dpy, win, &win_r, &win_p, &win_c, &nc)
	      && win_p != win_r) win = win_p;

	if (!(c = wintoclient(win))) return;

	a.ui = c->tags;
	view(&a);
}

void
hlwrapper(const Arg *arg){
    if (selmon->lt[selmon->sellt]->arrange == grid){
        Arg a = {.i = 2*((arg->f > 0) ? 1 : -1)};
        focusstack(&a);
    } else{
        setmfact(arg);
    }
}

