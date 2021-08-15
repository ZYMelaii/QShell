#include "parser.h"
#include "utf8str.h"

word_t* qshp_word_alloc()
{
	word_t *pw = qsh_malloc(sizeof(word_t));

	if (pw != NULL)
	{
		pw->flags = 0;
		pw->word = NULL;
	}

	return pw;
}

word_t* qshp_word_makebare(word_t *pw, const char *s)
{	//! unsafe
	if (pw == NULL || s == NULL) return pw;

	pw->word = qsh_strdup(s);

	return pw;
}

word_t* qshp_word_makeflags(word_t *pw, const char *s)
{	//! unsafe
	if (pw == NULL) return pw;
	if (pw->word == NULL && s == NULL) return pw;

	register const char *p = (pw->word != NULL ? pw->word : s);

	while (*p != '\0')
	{
		switch (*p)
		{
			case '$': pw->flags |= WORD_HASDOLLAR; break;
			case '\'':
			case '"':
			case '`': pw->flags |= WORD_QUOTED; break;
			case '\\': break;
		}
		p = qsh_utf8_next(p);
	}

	return pw;
}

word_t* qshp_word_make(const char *s)
{
	if (s == NULL) return NULL;

	word_t *pw = qshp_word_alloc();
	qshp_word_makebare(pw, s);
	qshp_word_makeflags(pw, s);

	return pw;
}

word_t* qshp_word_copy(word_t *pw)
{
	if (pw == NULL || pw->word == NULL) return NULL;

	word_t *npw = qshp_word_alloc();
	qshp_word_makebare(npw, pw->word);
	npw->flags = pw->flags;

	return npw;
}

void qshp_word_free(word_t *pw)
{
	if (pw == NULL) return;

	if (pw->word != NULL)
	{
		qsh_free(pw->word);
		pw->word = NULL;
	}

	pw->flags = 0;
}

wordlist_t* qshp_wordlist_make(word_t *pw, wordlist_t *pwl_next)
{	//! unsafe
	if (pw == NULL) return NULL;

	wordlist_t *pwl = qsh_malloc(sizeof(wordlist_t));

	pwl->word = pw;
	pwl->next = pwl_next;

	return pwl;
}

wordlist_t* qshp_wordlist_copy(wordlist_t *pwl)
{	//! unsafe
	if (pwl == NULL) return NULL;

	wordlist_t root, *p = &root;
	p->next = NULL;

	while (pwl)
	{
		p->next = qshp_wordlist_make(qshp_word_copy(pwl->word), NULL);
		p = p->next;
		pwl = pwl->next;
	}

	return root.next;
}

void qshp_wordlist_free(wordlist_t *pwl)
{
	if (pwl == NULL) return;

	wordlist_t root, *p = &root;
	p->next = pwl;

	while (p->next)
	{
		qshp_word_free(p->next->word);
		wordlist_t *tmp = p->next->next;
		qsh_free(p->next);
		p->next = tmp;
	}
}