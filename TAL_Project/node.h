#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdlib.h>

template <class T>
class node
{
public:
	T m_tData;
	node<T> *m_pPere,
		*m_pFilsg,
		*m_pFilsd;

	node(const T & d = NULL)
	{
		m_tData = d;
		m_pPere = m_pFilsg = m_pFilsd = NULL;
	}
	node(const T & d, node<T> * dad)
	{
		m_tData = d;
		m_pPere = dad;
		m_pFilsg = m_pFilsd = NULL;
	}

	static void detruire_arbre(node<T> *arbre)
	{
		if (arbre != NULL)
		{
			detruire_arbre(arbre->m_pFilsg);
			detruire_arbre(arbre->m_pFilsd);
			delete arbre;
		}
	}
	static int profondeur_arbre(const node<T> *arbre)
	{
		if (arbre == NULL)
			return 0;

		int iProfGauche = profondeur_arbre(arbre->m_pFilsg);
		int iProfDroit = profondeur_arbre(arbre->m_pFilsd);
		return 1 + ((iProfGauche > iProfDroit) ? iProfGauche : iProfDroit);
	}
	static int compter_feuilles_arbre(const node<T> *arbre)
	{
		if (arbre == NULL)
			return 0;
		else if (arbre->m_pFilsg == NULL && arbre->m_pFilsd == NULL)
			return 1;
		else
			return compter_feuilles_arbre(arbre->m_pFilsg) +
			compter_feuilles_arbre(arbre->m_pFilsd);
	}

	static void parcours_prefixe(const node<T> *arbre, void(*f) (const T &))
	{
		if (arbre != NULL)
		{
			f(arbre->m_tData);
			parcours_prefixe(arbre->m_pFilsg, f);
			parcours_prefixe(arbre->m_pFilsd, f);
		}
	}
	static void parcours_infixe(const node<T> *arbre, void(*f) (const T &))
	{
		if (arbre != NULL)
		{
			parcours_infixe(arbre->m_pFilsg, f);
			f(arbre->m_tData);
			parcours_infixe(arbre->m_pFilsd, f);
		}
	}
	static void parcours_suffixe(const node<T> *arbre, void(*f) (const T &))
	{
		if (arbre != NULL)
		{
			parcours_suffixe(arbre->m_pFilsg, f);
			parcours_suffixe(arbre->m_pFilsd, f);
			f(arbre->m_tData);
		}
	}

	static void parcours_prefixe(node<T> *arbre, void(*f) (T &))
	{
		if (arbre != NULL)
		{
			f(arbre->m_tData);
			parcours_prefixe(arbre->m_pFilsg, f);
			parcours_prefixe(arbre->m_pFilsd, f);
		}
	}
	static void parcours_infixe(node<T> *arbre, void(*f) (T &))
	{
		if (arbre != NULL)
		{
			parcours_infixe(arbre->m_pFilsg, f);
			f(arbre->m_tData);
			parcours_infixe(arbre->m_pFilsd, f);
		}
	}
	static void parcours_suffixe(node<T> *arbre, void(*f) (T &))
	{
		if (arbre != NULL)
		{
			parcours_suffixe(arbre->m_pFilsg, f);
			parcours_suffixe(arbre->m_pFilsd, f);
			f(arbre->m_tData);
		}
	}

	static void parcours_suffixe(node<T> *arbre, void(*f)(node<T> *))
	{
		if (arbre != NULL)
		{
			parcours_suffixe(arbre->m_pFilsg, f);
			parcours_suffixe(arbre->m_pFilsd, f);
			f(arbre);
		}
	}

	static node<T> *maximum(node<T> *arbre)
	{
		if (arbre == NULL)
			return NULL;

		while (arbre->m_pFilsd != NULL)
			arbre = arbre->m_pFilsd;
		return arbre;
	}
	static node<T> *minimum(node<T> *arbre)
	{
		if (arbre == NULL)
			return NULL;

		while (arbre->m_pFilsg != NULL)
			arbre = arbre->m_pFilsg;
		return arbre;
	}
};

template <class T>
class bintree
{
protected:
	node<T> * m_pRacine;
	int m_iNbNoeuds;

	bintree(const bintree &);
	bintree& operator = (const bintree &);

public:
	bintree()
	{
		m_pRacine = NULL;
		m_iNbNoeuds = 0;
	}
	virtual inline ~bintree()
	{
		if (m_iNbNoeuds > 0)
		{
			node<T>::detruire_arbre(m_pRacine->m_pFilsg);
			node<T>::detruire_arbre(m_pRacine->m_pFilsd);
			delete this->m_pRacine;
		}
	}

	inline int size() const
	{
		return m_iNbNoeuds;
	}
	inline int count_leaves() const
	{
		return node<T>::compter_feuilles_arbre(m_pRacine);
	}
	inline int depth() const
	{
		return node<T>::profondeur_arbre(m_pRacine);
	}

	virtual bool add(const T &) = 0;
	virtual bool remove(const T &) = 0;

	virtual void empty()
	{
		if (m_iNbNoeuds > 0)
		{
			node<T>::detruire_arbre(m_pRacine->m_pFilsg);
			node<T>::detruire_arbre(m_pRacine->m_pFilsd);
			delete m_pRacine;
			m_iNbNoeuds = 0;
		}
	}

	inline void prefix(void(*f) (T &))
	{
		node<T>::parcours_prefixe(m_pRacine, f);
	}
	inline void infix(void(*f) (T &))
	{
		node<T>::parcours_infixe(m_pRacine, f);
	}
	inline void suffix(void(*f) (T &))
	{
		node<T>::parcours_suffixe(m_pRacine, f);
	}

	inline void prefix(void(*f) (const T &)) const
	{
		node<T>::parcours_prefixe(m_pRacine, f);
	}
	inline void infix(void(*f) (const T &)) const
	{
		node<T>::parcours_infixe(m_pRacine, f);
	}
	inline void suffix(void(*f) (const T &)) const
	{
		node<T>::parcours_suffixe(m_pRacine, f);
	}

	inline void suffix(void(*f)(node<T> *)){
		node<T>::parcours_suffixe(m_pRacine, f);
	}
};

//-----------------------------------------------------------------------
//							CLASSE SEARCH_TREE
//-----------------------------------------------------------------------
// Cette classe permet de creer des arbres binaires de fouille de
// n'importe quel type. Le type choisit doit permettre le (=), le (==) et le (<).
//-----------------------------------------------------------------------
template <class T>
class search_tree : public bintree<T>
{
protected:
	search_tree(const search_tree &);
	search_tree& operator = (const search_tree &);

	node<T> * trouver_valeur(const T &element) const
	{
		node<T> *noeud = this->m_pRacine;
		while (noeud != NULL && !(*(noeud->m_tData) == *element))
			if (*element < *(noeud->m_tData))
				noeud = noeud->m_pFilsg;
			else
				noeud = noeud->m_pFilsd;
		return noeud;
	}
	node<T> * trouver_pere(const T &element) const
	{
		if (this->m_iNbNoeuds == 0)
			return NULL;

		node<T> * m_pPere = this->m_pRacine;
		bool pere_trouve = false;
		do
		{
			if (*element < *(m_pPere->m_tData) && m_pPere->m_pFilsg != NULL)
				m_pPere = m_pPere->m_pFilsg;
			else if (*(m_pPere->m_tData) < *element && m_pPere->m_pFilsd != NULL)
				m_pPere = m_pPere->m_pFilsd;
			else
				pere_trouve = true;
		} while (!pere_trouve);
		return m_pPere;
	}

public:
	search_tree()
	{

	}
	virtual inline ~search_tree()
	{

	}

	virtual bool add(const T &element)
	{
		if (this->present(element))
			return false;

		node<T> * m_pPere = this->trouver_pere(element);
		node<T> * nouveau = new node<T>(element, m_pPere);
		if (nouveau == NULL)
			return false;

		if (m_pPere == NULL)
			this->m_pRacine = nouveau;
		else if (*element < *(m_pPere->m_tData))
			m_pPere->m_pFilsg = nouveau;
		else
			m_pPere->m_pFilsd = nouveau;
		++this->m_iNbNoeuds;
		return true;
	}
	virtual bool remove(const T &element)
	{
		node<T> * noeud = this->trouver_valeur(element);
		if (noeud == NULL)
			return false;

		node<T> * remplacant = node<T>::maximum(noeud->m_pFilsg);

		if (remplacant == NULL)
		{
			if (noeud == this->m_pRacine)
				this->m_pRacine = noeud->m_pFilsd;
			else if (noeud->m_pPere->m_pFilsg == noeud)
				noeud->m_pPere->m_pFilsg = noeud->m_pFilsd;
			else
				noeud->m_pPere->m_pFilsd = noeud->m_pFilsd;
			if (noeud->m_pFilsd != NULL)
				noeud->m_pFilsd->m_pPere = noeud->m_pPere;
		}
		else	// J'ai un maximum des minima.
		{
			noeud->m_tData = remplacant->m_tData;
			if (remplacant->m_pPere->m_pFilsg == remplacant)
				remplacant->m_pPere->m_pFilsg = remplacant->m_pFilsg;
			else
				remplacant->m_pPere->m_pFilsg = remplacant->m_pFilsg;
			if (remplacant->m_pFilsg != NULL)
				remplacant->m_pFilsg->m_pPere = remplacant->m_pPere;
			noeud = remplacant;
		}
		delete noeud;
		--this->m_iNbNoeuds;
		return true;
	}

	inline bool present(const T & element) const
	{
		return (trouver_valeur(element) != NULL);
	}

	bool maximum(T &element) const
	{
		node<T> * noeud = node<T>::maximum(this->m_pRacine);
		if (noeud == NULL)
			return false;

		element = noeud->m_tData;
		return true;
	}
	bool minimum(T &element) const
	{
		node<T> * noeud = node<T>::minimum(this->m_pRacine);
		if (noeud == NULL)
			return false;

		element = noeud->m_tData;
		return true;
	}
};

//-----------------------------------------------------------------------
//							CLASSE AVL_TREE
//-----------------------------------------------------------------------
// Cette classe permet de creer des arbres binaires de fouille balances
// (AVL) de n'importe quel type.
// T doit supporter les operateurs : =, < et ==.
//-----------------------------------------------------------------------
template <class T>
class AVL_tree : public search_tree<T>
{
private:
	AVL_tree(const AVL_tree &);
	AVL_tree& operator=(const AVL_tree &);

	void balance_tree(node<T> *arbre)
	{
		if (arbre == NULL)
			return;

		int balance_racine = node<T>::profondeur_arbre(arbre->m_pFilsd) - node<T>::profondeur_arbre(arbre->m_pFilsg);
		if (balance_racine == 2)
		{
			int balance_filsd = node<T>::profondeur_arbre(arbre->m_pFilsd->m_pFilsd) - node<T>::profondeur_arbre(arbre->m_pFilsd->m_pFilsg);
			if (balance_filsd == -1)
				this->rotation_droite(arbre->m_pFilsd);
			this->rotation_gauche(arbre);
		}
		else if (balance_racine == -2)
		{
			int balance_filsg = node<T>::profondeur_arbre(arbre->m_pFilsg->m_pFilsd) - node<T>::profondeur_arbre(arbre->m_pFilsg->m_pFilsg);
			if (balance_filsg == 1)
				this->rotation_gauche(arbre->m_pFilsg);
			this->rotation_droite(arbre);
		}
		this->balance_tree(arbre->m_pPere);
	}
	void rotation_gauche(node<T> *arbre)
	{
		if (arbre == NULL)
			return;

		node<T> * filsdroit = arbre->m_pFilsd;
		if (arbre->m_pPere != NULL)
		{
			if (arbre->m_pPere->m_pFilsd == arbre)
				arbre->m_pPere->m_pFilsd = filsdroit;
			else
				arbre->m_pPere->m_pFilsg = filsdroit;
		}
		filsdroit->m_pPere = arbre->m_pPere;

		if (filsdroit->m_pFilsg != NULL)
			filsdroit->m_pFilsg->m_pPere = arbre;
		arbre->m_pFilsd = filsdroit->m_pFilsg;

		arbre->m_pPere = filsdroit;
		filsdroit->m_pFilsg = arbre;
		if (arbre == this->m_pRacine)
			this->m_pRacine = filsdroit;
	}
	void rotation_droite(node<T> *arbre)
	{
		if (arbre == NULL)			// Si l'arbre est vide, on ne fait rien.
			return;

		node<T> * filsgauche = arbre->m_pFilsg;
		// On attache le parent de arbre au fils gauche de arbre.
		if (arbre->m_pPere != NULL)
		{
		    if (arbre->m_pPere->m_pFilsg == arbre)
				arbre->m_pPere->m_pFilsg = filsgauche;
			else
				arbre->m_pPere->m_pFilsd = filsgauche;
		}
		filsgauche->m_pPere = arbre->m_pPere;

		// On lie arbre et le sous arbre droit du fils gauche.
		if (filsgauche->m_pFilsd != NULL)
			filsgauche->m_pFilsd->m_pPere = arbre;
		arbre->m_pFilsg = filsgauche->m_pFilsd;

		// On place le fils droit comme sommet de arbre.
		arbre->m_pPere = filsgauche;
		filsgauche->m_pFilsd = arbre;
		if (arbre == this->m_pRacine)
			this->m_pRacine = filsgauche;
	}

public:
	AVL_tree()
	{

	}
	~AVL_tree()
	{

	}

	bool add(const T &element)
	{
		if (!search_tree<T>::add(element))
			return false;

		this->balance_tree(this->trouver_valeur(element));
		return true;
	}
	bool remove(const T &element)
	{
		node<T> * parent = this->trouver_valeur(element);
		parent = parent->m_pPere;

		if (search_tree<T>::remove(element) == 0)
			return false;

		this->balance_tree(parent);
		return true;
	}
	T findElement(const T& element) const{
		node<T> *pNode = search_tree<T>::trouver_valeur(element);
		if (pNode == NULL)
			return NULL;

		return pNode->m_tData;
	}
};

#endif
