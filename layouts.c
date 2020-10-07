void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster
			? m->ww * (m->rmaster ? 1.0 - m->mfact : m->mfact)
			: 0;
	else
		mw = m->ww;
	for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->rmaster ? m->wx + m->ww - mw : m->wx,
			       m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
			my += HEIGHT(c);
		} else {
			h = (m->wh - ty) / (n - i);
			resize(c, m->rmaster ? m->wx : m->wx + mw, m->wy + ty,
			       m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
			ty += HEIGHT(c);
		}
}

static void
bstack(Monitor *m) {
	int w, mh, mx, tx;
	unsigned int i, n;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	if (n > m->nmaster) {
		mh = m->nmaster 
            ? (m->rmaster ?  1.0 - m->mfact : m->mfact) * m->wh : 0;
	} else {
		mh = m->wh;
	}
	for (i = mx = 0, tx = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->rmaster ? m->wy + m->wh - mh : m->wy,
                    w - (2 * c->bw), mh - (2 * c->bw), 0);
			mx += WIDTH(c);
		} else {
			w = (m->ww - tx) / (n - i);
			resize(c, m->wx + tx, m->rmaster ? m->wy : m->wy + mh, w - (2 * c->bw), m->wh - mh - (2 * c->bw), 0);
            tx += WIDTH(c);
		}
	}
}

void
monocle(Monitor *m)
{
	unsigned int n = 0;
	Client *c;

	for (c = m->clients; c; c = c->next)
		if (ISVISIBLE(c))
			n++;
	if (n > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
	for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
}

void
grid(Monitor *m) {
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

void
deck(Monitor *m) {
	unsigned int i, n, mw;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if(n == 0)
		return;

	if(n > m->nmaster) {
		mw = m->nmaster
			? m->ww * (m->rmaster ? 1.0 - m->mfact : m->mfact)
			: 0;
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "%d|%d", m->nmaster, n - m->nmaster);
	}
	else{
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "%d|%d", n, 0);
		mw = m->ww;
    }
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if(i < m->nmaster) {
			resize(c, m->rmaster ? m->wx + m->ww - mw : m->wx,
					m->wy, mw - (2*c->bw), m->wh - (2*c->bw), 0);
		}
		else
			resize(c, m->rmaster ? m->wx : m->wx + mw, m->wy,
					m->ww - mw - (2*c->bw), m->wh - (2*c->bw), 0);
}

///////////////////////////////////
/*  Helper functions start here  */
///////////////////////////////////
void getPos(int *start, int *num, int *pos, int sep){
    Client *c;
    int END;
    for (*pos=0, c = nexttiled(selmon->clients); c != nexttiled(selmon->sel); (*pos)++, c = nexttiled(c->next));
    for (END=*pos, c = nexttiled(c->next); c; END++, c = nexttiled(c->next));
    sep = (sep > END) ? END + 1: sep;
    *start = ( *pos < sep || sep < 0 ) ? 0 : sep;
    *num = ( *pos >= sep || sep < 0 ) ?  END - *start + 1: sep;
}

void 
focusmain(int a, Client **c){
    int start, num, pos, j;
    getPos(&start, &num, &pos, -1);
    for (*c = selmon->clients, j=-start; j < ((num + pos + a - start) % (num)); j++, *c = nexttiled((*c)->next));
}

void 
focusgrid(int a, Client **cp){
    Client *c=NULL;
    int pos, end, h, i;
    for (pos=0, c = nexttiled(selmon->clients); c!= selmon->sel; pos++, c = nexttiled(c->next));
    for (end=pos, c = nexttiled(c->next); c; end++, c = nexttiled(c->next));
    for (h=0; h*h<end+1; h++);
    if (a == 1 && (pos % h != h-1 && pos != end)){
            focusmain(a, &c);
    }
    else if (a==-1 && pos % h){
            focusmain(a, &c);
    }
    else if (a==2 && end-pos>=h){
        for (i=0; i<h; i++){
            focusmain(a/2, &c);
        }
    }
    else if (a==-2 && pos >= h){
        for (i=0; i<h; i++){
            focusmain(a/2, &c);
        }
    }
    *cp = c;
}
