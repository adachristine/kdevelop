/* $ANTLR 2.7.1: "java.store.g" -> "JavaStoreWalker.cpp"$ */
#include "JavaStoreWalker.hpp"
#include "antlr/Token.hpp"
#include "antlr/AST.hpp"
#include "antlr/NoViableAltException.hpp"
#include "antlr/MismatchedTokenException.hpp"
#include "antlr/SemanticException.hpp"
#include "antlr/BitSet.hpp"
#line 1 "java.store.g"

#line 12 "JavaStoreWalker.cpp"
JavaStoreWalker::JavaStoreWalker()
	: ANTLR_USE_NAMESPACE(antlr)TreeParser() {
	setTokenNames(_tokenNames);
}

void JavaStoreWalker::compilationUnit(RefJavaAST _t) {
	RefJavaAST compilationUnit_AST_in = _t;
#line 67 "java.store.g"
	ParsedClass* kl;
#line 22 "JavaStoreWalker.cpp"
	
	try {      // for error handling
#line 68 "java.store.g"
		init();
#line 27 "JavaStoreWalker.cpp"
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case PACKAGE_DEF:
		{
			packageDefinition(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case CLASS_DEF:
		case INTERFACE_DEF:
		case IMPORT:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==IMPORT)) {
				importDefinition(_t);
				_t = _retTree;
			}
			else {
				goto _loop4;
			}
			
		}
		_loop4:;
		}
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==CLASS_DEF||_t->getType()==INTERFACE_DEF)) {
				kl=typeDefinition(_t);
				_t = _retTree;
#line 71 "java.store.g"
				m_store->addClass( kl );
#line 75 "JavaStoreWalker.cpp"
			}
			else {
				goto _loop6;
			}
			
		}
		_loop6:;
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::packageDefinition(RefJavaAST _t) {
	RefJavaAST packageDefinition_AST_in = _t;
#line 74 "java.store.g"
	QString id;
#line 97 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t8 = _t;
		RefJavaAST tmp1_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PACKAGE_DEF);
		_t = _t->getFirstChild();
		id=identifier(_t);
		_t = _retTree;
		_t = __t8;
		_t = _t->getNextSibling();
#line 75 "java.store.g"
		m_package = id;
#line 110 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

 QString  JavaStoreWalker::importDefinition(RefJavaAST _t) {
#line 78 "java.store.g"
	 QString id ;
#line 123 "JavaStoreWalker.cpp"
	RefJavaAST importDefinition_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t10 = _t;
		RefJavaAST tmp2_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IMPORT);
		_t = _t->getFirstChild();
		id=identifierStar(_t);
		_t = _retTree;
		_t = __t10;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return id ;
}

ParsedClass*  JavaStoreWalker::typeDefinition(RefJavaAST _t) {
#line 82 "java.store.g"
	ParsedClass* klass ;
#line 148 "JavaStoreWalker.cpp"
	RefJavaAST typeDefinition_AST_in = _t;
	RefJavaAST m = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	RefJavaAST mm = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
#line 82 "java.store.g"
	QStringList bases; klass=0;
#line 154 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case CLASS_DEF:
		{
			RefJavaAST __t12 = _t;
			RefJavaAST tmp3_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CLASS_DEF);
			_t = _t->getFirstChild();
			m = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			modifiers(_t);
			_t = _retTree;
			RefJavaAST tmp4_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
#line 84 "java.store.g"
			
									klass = new ParsedClass; 
									klass->setName( tmp4_AST_in->getText().c_str() );
									klass->setDeclaredInFile( getFilename().c_str() );
									klass->setDefinedInFile( getFilename().c_str() );
									klass->setDeclaredOnLine( tmp4_AST_in->getLine() );
									klass->setDefinedOnLine( tmp4_AST_in->getLine() );
									kdDebug() << "klass->path = " << klass->path() << endl;
									
#line 182 "JavaStoreWalker.cpp"
			bases=extendsClause(_t);
			_t = _retTree;
#line 94 "java.store.g"
			
									QStringList::Iterator it = bases.begin();
									while( it != bases.end() ){
										ParsedParent* parent = new ParsedParent;
										parent->setName( *it );
										klass->addParent( parent );
										++it;
									}
									
#line 195 "JavaStoreWalker.cpp"
			implementsClause(_t);
			_t = _retTree;
			objBlock(_t,klass);
			_t = _retTree;
			_t = __t12;
			_t = _t->getNextSibling();
			break;
		}
		case INTERFACE_DEF:
		{
			RefJavaAST __t13 = _t;
			RefJavaAST tmp5_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),INTERFACE_DEF);
			_t = _t->getFirstChild();
			mm = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			modifiers(_t);
			_t = _retTree;
			RefJavaAST tmp6_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
#line 106 "java.store.g"
			
									klass = new ParsedClass; 
									klass->setName( tmp6_AST_in->getText().c_str() );
									klass->setDeclaredInFile( getFilename().c_str() );
									klass->setDeclaredOnLine( tmp6_AST_in->getLine() );
									klass->setDefinedOnLine( tmp6_AST_in->getLine() )
			;						kdDebug() << "klass->path = " << klass->path() << endl;
									
#line 225 "JavaStoreWalker.cpp"
			bases=extendsClause(_t);
			_t = _retTree;
#line 115 "java.store.g"
			
									QStringList::Iterator it = bases.begin();
									while( it != bases.end() ){
										ParsedParent* parent = new ParsedParent;
										parent->setName( *it );
										klass->addParent( parent );
										++it;
									}
									
#line 238 "JavaStoreWalker.cpp"
			interfaceBlock(_t,klass);
			_t = _retTree;
			_t = __t13;
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return klass ;
}

 QString  JavaStoreWalker::identifier(RefJavaAST _t) {
#line 286 "java.store.g"
	 QString id ;
#line 263 "JavaStoreWalker.cpp"
	RefJavaAST identifier_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IDENT:
		{
			RefJavaAST tmp7_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
#line 287 "java.store.g"
			id = tmp7_AST_in->getText().c_str();
#line 277 "JavaStoreWalker.cpp"
			break;
		}
		case DOT:
		{
			RefJavaAST __t74 = _t;
			RefJavaAST tmp8_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DOT);
			_t = _t->getFirstChild();
			id=identifier(_t);
			_t = _retTree;
			RefJavaAST tmp9_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
			_t = __t74;
			_t = _t->getNextSibling();
#line 288 "java.store.g"
			id += QString(".") + tmp9_AST_in->getText().c_str();
#line 295 "JavaStoreWalker.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return id ;
}

 QString  JavaStoreWalker::identifierStar(RefJavaAST _t) {
#line 291 "java.store.g"
	 QString id ;
#line 316 "JavaStoreWalker.cpp"
	RefJavaAST identifierStar_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IDENT:
		{
			RefJavaAST tmp10_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
#line 292 "java.store.g"
			id = tmp10_AST_in->getText().c_str();
#line 330 "JavaStoreWalker.cpp"
			break;
		}
		case DOT:
		{
			RefJavaAST __t76 = _t;
			RefJavaAST tmp11_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DOT);
			_t = _t->getFirstChild();
			id=identifier(_t);
			_t = _retTree;
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case STAR:
			{
				RefJavaAST tmp12_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STAR);
				_t = _t->getNextSibling();
#line 293 "java.store.g"
				id += QString(".") + tmp12_AST_in->getText().c_str();
#line 352 "JavaStoreWalker.cpp"
				break;
			}
			case IDENT:
			{
				RefJavaAST tmp13_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
				_t = _t->getNextSibling();
#line 294 "java.store.g"
				id += QString(".") + tmp13_AST_in->getText().c_str();
#line 362 "JavaStoreWalker.cpp"
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t76;
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return id ;
}

 QStringList  JavaStoreWalker::modifiers(RefJavaAST _t) {
#line 153 "java.store.g"
	 QStringList l ;
#line 393 "JavaStoreWalker.cpp"
	RefJavaAST modifiers_AST_in = _t;
	RefJavaAST m = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		RefJavaAST __t21 = _t;
		RefJavaAST tmp14_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),MODIFIERS);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_0.member(_t->getType()))) {
				m = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
				modifier(_t);
				_t = _retTree;
#line 154 "java.store.g"
				l << m->getText().c_str();
#line 412 "JavaStoreWalker.cpp"
			}
			else {
				goto _loop23;
			}
			
		}
		_loop23:;
		}
		_t = __t21;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return l ;
}

 QStringList  JavaStoreWalker::extendsClause(RefJavaAST _t) {
#line 172 "java.store.g"
	 QStringList l ;
#line 436 "JavaStoreWalker.cpp"
	RefJavaAST extendsClause_AST_in = _t;
#line 172 "java.store.g"
	QString id;
#line 440 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t26 = _t;
		RefJavaAST tmp15_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EXTENDS_CLAUSE);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==IDENT||_t->getType()==DOT)) {
				id=identifier(_t);
				_t = _retTree;
#line 173 "java.store.g"
				l << id;
#line 456 "JavaStoreWalker.cpp"
			}
			else {
				goto _loop28;
			}
			
		}
		_loop28:;
		}
		_t = __t26;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return l ;
}

 QStringList  JavaStoreWalker::implementsClause(RefJavaAST _t) {
#line 176 "java.store.g"
	 QStringList l ;
#line 480 "JavaStoreWalker.cpp"
	RefJavaAST implementsClause_AST_in = _t;
#line 176 "java.store.g"
	QString id;
#line 484 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t30 = _t;
		RefJavaAST tmp16_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IMPLEMENTS_CLAUSE);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==IDENT||_t->getType()==DOT)) {
				id=identifier(_t);
				_t = _retTree;
#line 177 "java.store.g"
				l << id;
#line 500 "JavaStoreWalker.cpp"
			}
			else {
				goto _loop32;
			}
			
		}
		_loop32:;
		}
		_t = __t30;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return l ;
}

void JavaStoreWalker::objBlock(RefJavaAST _t,
	 ParsedClass* klass 
) {
	RefJavaAST objBlock_AST_in = _t;
#line 189 "java.store.g"
	ParsedClass* kl; ParsedMethod* meth; ParsedAttribute* attr;
#line 527 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t38 = _t;
		RefJavaAST tmp17_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),OBJBLOCK);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case CTOR_DEF:
			{
				meth=ctorDef(_t);
				_t = _retTree;
#line 191 "java.store.g"
				klass->addMethod( meth );
#line 545 "JavaStoreWalker.cpp"
				break;
			}
			case METHOD_DEF:
			{
				meth=methodDef(_t);
				_t = _retTree;
#line 192 "java.store.g"
				klass->addMethod( meth );
#line 554 "JavaStoreWalker.cpp"
				break;
			}
			case VARIABLE_DEF:
			{
				attr=variableDef(_t);
				_t = _retTree;
#line 193 "java.store.g"
				klass->addAttribute( attr );
#line 563 "JavaStoreWalker.cpp"
				break;
			}
			case CLASS_DEF:
			case INTERFACE_DEF:
			{
				kl=typeDefinition(_t);
				_t = _retTree;
#line 194 "java.store.g"
				klass->addClass( kl );
#line 573 "JavaStoreWalker.cpp"
				break;
			}
			case STATIC_INIT:
			{
				RefJavaAST __t40 = _t;
				RefJavaAST tmp18_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STATIC_INIT);
				_t = _t->getFirstChild();
				slist(_t);
				_t = _retTree;
				_t = __t40;
				_t = _t->getNextSibling();
				break;
			}
			case INSTANCE_INIT:
			{
				RefJavaAST __t41 = _t;
				RefJavaAST tmp19_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),INSTANCE_INIT);
				_t = _t->getFirstChild();
				slist(_t);
				_t = _retTree;
				_t = __t41;
				_t = _t->getNextSibling();
				break;
			}
			default:
			{
				goto _loop42;
			}
			}
		}
		_loop42:;
		}
		_t = __t38;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::interfaceBlock(RefJavaAST _t,
	 ParsedClass* klass 
) {
	RefJavaAST interfaceBlock_AST_in = _t;
#line 181 "java.store.g"
	ParsedMethod* meth; ParsedAttribute* attr;
#line 625 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t34 = _t;
		RefJavaAST tmp20_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),OBJBLOCK);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case METHOD_DEF:
			{
				meth=methodDecl(_t);
				_t = _retTree;
#line 183 "java.store.g"
				klass->addMethod( meth );
#line 643 "JavaStoreWalker.cpp"
				break;
			}
			case VARIABLE_DEF:
			{
				attr=variableDef(_t);
				_t = _retTree;
#line 184 "java.store.g"
				klass->addAttribute( attr );
#line 652 "JavaStoreWalker.cpp"
				break;
			}
			default:
			{
				goto _loop36;
			}
			}
		}
		_loop36:;
		}
		_t = __t34;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

 QString  JavaStoreWalker::typeSpec(RefJavaAST _t) {
#line 127 "java.store.g"
	 QString tp ;
#line 677 "JavaStoreWalker.cpp"
	RefJavaAST typeSpec_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t15 = _t;
		RefJavaAST tmp21_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPE);
		_t = _t->getFirstChild();
		tp=typeSpecArray(_t);
		_t = _retTree;
		_t = __t15;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return tp ;
}

 QString  JavaStoreWalker::typeSpecArray(RefJavaAST _t) {
#line 131 "java.store.g"
	 QString tp ;
#line 702 "JavaStoreWalker.cpp"
	RefJavaAST typeSpecArray_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ARRAY_DECLARATOR:
		{
			RefJavaAST __t17 = _t;
			RefJavaAST tmp22_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ARRAY_DECLARATOR);
			_t = _t->getFirstChild();
			tp=typeSpecArray(_t);
			_t = _retTree;
			_t = __t17;
			_t = _t->getNextSibling();
#line 132 "java.store.g"
			tp += "[]";
#line 721 "JavaStoreWalker.cpp"
			break;
		}
		case LITERAL_void:
		case LITERAL_boolean:
		case LITERAL_byte:
		case LITERAL_char:
		case LITERAL_short:
		case LITERAL_int:
		case LITERAL_float:
		case LITERAL_long:
		case LITERAL_double:
		case IDENT:
		case DOT:
		{
			tp=type(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return tp ;
}

 QString  JavaStoreWalker::type(RefJavaAST _t) {
#line 136 "java.store.g"
	 QString tp ;
#line 758 "JavaStoreWalker.cpp"
	RefJavaAST type_AST_in = _t;
	RefJavaAST b = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IDENT:
		case DOT:
		{
			tp=identifier(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_void:
		case LITERAL_boolean:
		case LITERAL_byte:
		case LITERAL_char:
		case LITERAL_short:
		case LITERAL_int:
		case LITERAL_float:
		case LITERAL_long:
		case LITERAL_double:
		{
			b = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
			builtInType(_t);
			_t = _retTree;
#line 138 "java.store.g"
			tp = b->getText().c_str();
#line 788 "JavaStoreWalker.cpp"
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return tp ;
}

void JavaStoreWalker::builtInType(RefJavaAST _t) {
	RefJavaAST builtInType_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case LITERAL_void:
		{
			RefJavaAST tmp23_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_void);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_boolean:
		{
			RefJavaAST tmp24_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_boolean);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_byte:
		{
			RefJavaAST tmp25_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_byte);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_char:
		{
			RefJavaAST tmp26_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_char);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_short:
		{
			RefJavaAST tmp27_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_short);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_int:
		{
			RefJavaAST tmp28_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_int);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_float:
		{
			RefJavaAST tmp29_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_float);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_long:
		{
			RefJavaAST tmp30_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_long);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_double:
		{
			RefJavaAST tmp31_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_double);
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::modifier(RefJavaAST _t) {
	RefJavaAST modifier_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case LITERAL_private:
		{
			RefJavaAST tmp32_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_private);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_public:
		{
			RefJavaAST tmp33_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_public);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_protected:
		{
			RefJavaAST tmp34_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_protected);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_static:
		{
			RefJavaAST tmp35_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_static);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_transient:
		{
			RefJavaAST tmp36_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_transient);
			_t = _t->getNextSibling();
			break;
		}
		case FINAL:
		{
			RefJavaAST tmp37_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FINAL);
			_t = _t->getNextSibling();
			break;
		}
		case ABSTRACT:
		{
			RefJavaAST tmp38_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ABSTRACT);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_native:
		{
			RefJavaAST tmp39_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_native);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_threadsafe:
		{
			RefJavaAST tmp40_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_threadsafe);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_synchronized:
		{
			RefJavaAST tmp41_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_synchronized);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_const:
		{
			RefJavaAST tmp42_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_const);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_volatile:
		{
			RefJavaAST tmp43_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_volatile);
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

 ParsedMethod*  JavaStoreWalker::methodDecl(RefJavaAST _t) {
#line 211 "java.store.g"
	 ParsedMethod* meth ;
#line 998 "JavaStoreWalker.cpp"
	RefJavaAST methodDecl_AST_in = _t;
	RefJavaAST m = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
#line 211 "java.store.g"
	QString tp; meth = new ParsedMethod;
#line 1003 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t46 = _t;
		RefJavaAST tmp44_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),METHOD_DEF);
		_t = _t->getFirstChild();
		m = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
		modifiers(_t);
		_t = _retTree;
		tp=typeSpec(_t);
		_t = _retTree;
		methodHead(_t,meth);
		_t = _retTree;
		_t = __t46;
		_t = _t->getNextSibling();
#line 213 "java.store.g"
									
									meth->setDeclaredInFile( getFilename().c_str() );
									meth->setDefinedInFile( getFilename().c_str() );
									meth->setType( tp );
									
#line 1025 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return meth ;
}

 ParsedAttribute*  JavaStoreWalker::variableDef(RefJavaAST _t) {
#line 229 "java.store.g"
	 ParsedAttribute* attr ;
#line 1039 "JavaStoreWalker.cpp"
	RefJavaAST variableDef_AST_in = _t;
	RefJavaAST m = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
#line 229 "java.store.g"
	QString tp; attr = new ParsedAttribute;
#line 1044 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t51 = _t;
		RefJavaAST tmp45_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),VARIABLE_DEF);
		_t = _t->getFirstChild();
		m = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
		modifiers(_t);
		_t = _retTree;
		tp=typeSpec(_t);
		_t = _retTree;
		variableDeclarator(_t,attr);
		_t = _retTree;
		varInitializer(_t);
		_t = _retTree;
		_t = __t51;
		_t = _t->getNextSibling();
#line 231 "java.store.g"
		
									attr->setDeclaredInFile( getFilename().c_str() );
									attr->setDefinedInFile( getFilename().c_str() );
									attr->setType( tp );
									
#line 1068 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return attr ;
}

 ParsedMethod*  JavaStoreWalker::ctorDef(RefJavaAST _t) {
#line 201 "java.store.g"
	 ParsedMethod* meth ;
#line 1082 "JavaStoreWalker.cpp"
	RefJavaAST ctorDef_AST_in = _t;
	RefJavaAST m = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
#line 201 "java.store.g"
	meth = new ParsedMethod; meth->setIsConstructor( TRUE );
#line 1087 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t44 = _t;
		RefJavaAST tmp46_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CTOR_DEF);
		_t = _t->getFirstChild();
		m = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
		modifiers(_t);
		_t = _retTree;
		methodHead(_t,meth);
		_t = _retTree;
		slist(_t);
		_t = _retTree;
		_t = __t44;
		_t = _t->getNextSibling();
#line 205 "java.store.g"
									
									meth->setDeclaredInFile( getFilename().c_str() );
									meth->setDefinedInFile( getFilename().c_str() );
									
#line 1108 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return meth ;
}

 ParsedMethod*  JavaStoreWalker::methodDef(RefJavaAST _t) {
#line 220 "java.store.g"
	 ParsedMethod* meth ;
#line 1122 "JavaStoreWalker.cpp"
	RefJavaAST methodDef_AST_in = _t;
	RefJavaAST m = static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST);
#line 220 "java.store.g"
	QString tp; meth = new ParsedMethod;
#line 1127 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t48 = _t;
		RefJavaAST tmp47_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),METHOD_DEF);
		_t = _t->getFirstChild();
		m = (_t == ASTNULL) ? static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) : _t;
		modifiers(_t);
		_t = _retTree;
		tp=typeSpec(_t);
		_t = _retTree;
		methodHead(_t,meth);
		_t = _retTree;
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case SLIST:
		{
			slist(_t);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		_t = __t48;
		_t = _t->getNextSibling();
#line 222 "java.store.g"
		
									meth->setDeclaredInFile( getFilename().c_str() );	
									meth->setDefinedInFile( getFilename().c_str() );
									meth->setType( tp );
									
#line 1169 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return meth ;
}

void JavaStoreWalker::slist(RefJavaAST _t) {
	RefJavaAST slist_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t79 = _t;
		RefJavaAST tmp48_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SLIST);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_tokenSet_1.member(_t->getType()))) {
				stat(_t);
				_t = _retTree;
			}
			else {
				goto _loop81;
			}
			
		}
		_loop81:;
		}
		_t = __t79;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::methodHead(RefJavaAST _t,
	 ParsedMethod* meth 
) {
	RefJavaAST methodHead_AST_in = _t;
#line 273 "java.store.g"
	ParsedArgument* p;
#line 1220 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST tmp49_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
		_t = _t->getNextSibling();
		RefJavaAST __t65 = _t;
		RefJavaAST tmp50_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PARAMETERS);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==PARAMETER_DEF)) {
				p=parameterDef(_t);
				_t = _retTree;
#line 274 "java.store.g"
				meth->addArgument( p );
#line 1239 "JavaStoreWalker.cpp"
			}
			else {
				goto _loop67;
			}
			
		}
		_loop67:;
		}
		_t = __t65;
		_t = _t->getNextSibling();
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case LITERAL_throws:
		{
			throwsClause(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case SLIST:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
#line 275 "java.store.g"
		
				meth->setName( tmp49_AST_in->getText().c_str() );
				meth->setDeclaredOnLine( tmp49_AST_in->getLine() );
				meth->setDefinedOnLine( tmp49_AST_in->getLine() );
			
#line 1277 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::variableDeclarator(RefJavaAST _t,
	 ParsedAttribute* attr 
) {
	RefJavaAST variableDeclarator_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IDENT:
		{
			RefJavaAST tmp51_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
#line 251 "java.store.g"
			
										attr->setName( tmp51_AST_in->getText().c_str() );
										attr->setDeclaredOnLine( tmp51_AST_in->getLine() );
										attr->setDefinedOnLine( tmp51_AST_in->getLine() );
										
#line 1307 "JavaStoreWalker.cpp"
			break;
		}
		case LBRACK:
		{
			RefJavaAST tmp52_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LBRACK);
			_t = _t->getNextSibling();
			variableDeclarator(_t,attr);
			_t = _retTree;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::varInitializer(RefJavaAST _t) {
	RefJavaAST varInitializer_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ASSIGN:
		{
			RefJavaAST __t58 = _t;
			RefJavaAST tmp53_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ASSIGN);
			_t = _t->getFirstChild();
			initializer(_t);
			_t = _retTree;
			_t = __t58;
			_t = _t->getNextSibling();
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

 ParsedArgument*  JavaStoreWalker::parameterDef(RefJavaAST _t) {
#line 238 "java.store.g"
	 ParsedArgument* arg ;
#line 1373 "JavaStoreWalker.cpp"
	RefJavaAST parameterDef_AST_in = _t;
#line 238 "java.store.g"
	QString tp; arg = new ParsedArgument;
#line 1377 "JavaStoreWalker.cpp"
	
	try {      // for error handling
		RefJavaAST __t53 = _t;
		RefJavaAST tmp54_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PARAMETER_DEF);
		_t = _t->getFirstChild();
		modifiers(_t);
		_t = _retTree;
		tp=typeSpec(_t);
		_t = _retTree;
		RefJavaAST tmp55_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
		_t = _t->getNextSibling();
		_t = __t53;
		_t = _t->getNextSibling();
#line 240 "java.store.g"
									
									arg->setName( tmp55_AST_in->getText().c_str() );
									arg->setType( tp );
									
#line 1398 "JavaStoreWalker.cpp"
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
	return arg ;
}

void JavaStoreWalker::objectinitializer(RefJavaAST _t) {
	RefJavaAST objectinitializer_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t55 = _t;
		RefJavaAST tmp56_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),INSTANCE_INIT);
		_t = _t->getFirstChild();
		slist(_t);
		_t = _retTree;
		_t = __t55;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::initializer(RefJavaAST _t) {
	RefJavaAST initializer_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case EXPR:
		{
			expression(_t);
			_t = _retTree;
			break;
		}
		case ARRAY_INIT:
		{
			arrayInitializer(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::expression(RefJavaAST _t) {
	RefJavaAST expression_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t124 = _t;
		RefJavaAST tmp57_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EXPR);
		_t = _t->getFirstChild();
		expr(_t);
		_t = _retTree;
		_t = __t124;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::arrayInitializer(RefJavaAST _t) {
	RefJavaAST arrayInitializer_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t61 = _t;
		RefJavaAST tmp58_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ARRAY_INIT);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==EXPR||_t->getType()==ARRAY_INIT)) {
				initializer(_t);
				_t = _retTree;
			}
			else {
				goto _loop63;
			}
			
		}
		_loop63:;
		}
		_t = __t61;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::throwsClause(RefJavaAST _t) {
	RefJavaAST throwsClause_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t70 = _t;
		RefJavaAST tmp59_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_throws);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==IDENT||_t->getType()==DOT)) {
				identifier(_t);
				_t = _retTree;
			}
			else {
				goto _loop72;
			}
			
		}
		_loop72:;
		}
		_t = __t70;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::stat(RefJavaAST _t) {
	RefJavaAST stat_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case CLASS_DEF:
		case INTERFACE_DEF:
		{
			typeDefinition(_t);
			_t = _retTree;
			break;
		}
		case VARIABLE_DEF:
		{
			variableDef(_t);
			_t = _retTree;
			break;
		}
		case EXPR:
		{
			expression(_t);
			_t = _retTree;
			break;
		}
		case LABELED_STAT:
		{
			RefJavaAST __t83 = _t;
			RefJavaAST tmp60_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LABELED_STAT);
			_t = _t->getFirstChild();
			RefJavaAST tmp61_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
			stat(_t);
			_t = _retTree;
			_t = __t83;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_if:
		{
			RefJavaAST __t84 = _t;
			RefJavaAST tmp62_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_if);
			_t = _t->getFirstChild();
			expression(_t);
			_t = _retTree;
			stat(_t);
			_t = _retTree;
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case SLIST:
			case VARIABLE_DEF:
			case CLASS_DEF:
			case INTERFACE_DEF:
			case LABELED_STAT:
			case EXPR:
			case EMPTY_STAT:
			case LITERAL_synchronized:
			case LITERAL_if:
			case LITERAL_for:
			case LITERAL_while:
			case LITERAL_do:
			case LITERAL_break:
			case LITERAL_continue:
			case LITERAL_return:
			case LITERAL_switch:
			case LITERAL_throw:
			case LITERAL_try:
			{
				stat(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t84;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_for:
		{
			RefJavaAST __t86 = _t;
			RefJavaAST tmp63_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_for);
			_t = _t->getFirstChild();
			RefJavaAST __t87 = _t;
			RefJavaAST tmp64_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FOR_INIT);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case VARIABLE_DEF:
			{
				variableDef(_t);
				_t = _retTree;
				break;
			}
			case ELIST:
			{
				elist(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t87;
			_t = _t->getNextSibling();
			RefJavaAST __t89 = _t;
			RefJavaAST tmp65_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FOR_CONDITION);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case EXPR:
			{
				expression(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t89;
			_t = _t->getNextSibling();
			RefJavaAST __t91 = _t;
			RefJavaAST tmp66_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),FOR_ITERATOR);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case ELIST:
			{
				elist(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t91;
			_t = _t->getNextSibling();
			stat(_t);
			_t = _retTree;
			_t = __t86;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_while:
		{
			RefJavaAST __t93 = _t;
			RefJavaAST tmp67_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_while);
			_t = _t->getFirstChild();
			expression(_t);
			_t = _retTree;
			stat(_t);
			_t = _retTree;
			_t = __t93;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_do:
		{
			RefJavaAST __t94 = _t;
			RefJavaAST tmp68_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_do);
			_t = _t->getFirstChild();
			stat(_t);
			_t = _retTree;
			expression(_t);
			_t = _retTree;
			_t = __t94;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_break:
		{
			RefJavaAST __t95 = _t;
			RefJavaAST tmp69_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_break);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case IDENT:
			{
				RefJavaAST tmp70_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
				_t = _t->getNextSibling();
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t95;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_continue:
		{
			RefJavaAST __t97 = _t;
			RefJavaAST tmp71_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_continue);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case IDENT:
			{
				RefJavaAST tmp72_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
				_t = _t->getNextSibling();
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t97;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_return:
		{
			RefJavaAST __t99 = _t;
			RefJavaAST tmp73_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_return);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case EXPR:
			{
				expression(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t99;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_switch:
		{
			RefJavaAST __t101 = _t;
			RefJavaAST tmp74_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_switch);
			_t = _t->getFirstChild();
			expression(_t);
			_t = _retTree;
			{
			for (;;) {
				if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				if ((_t->getType()==CASE_GROUP)) {
					caseGroup(_t);
					_t = _retTree;
				}
				else {
					goto _loop103;
				}
				
			}
			_loop103:;
			}
			_t = __t101;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_throw:
		{
			RefJavaAST __t104 = _t;
			RefJavaAST tmp75_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_throw);
			_t = _t->getFirstChild();
			expression(_t);
			_t = _retTree;
			_t = __t104;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_synchronized:
		{
			RefJavaAST __t105 = _t;
			RefJavaAST tmp76_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_synchronized);
			_t = _t->getFirstChild();
			expression(_t);
			_t = _retTree;
			stat(_t);
			_t = _retTree;
			_t = __t105;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_try:
		{
			tryBlock(_t);
			_t = _retTree;
			break;
		}
		case SLIST:
		{
			slist(_t);
			_t = _retTree;
			break;
		}
		case EMPTY_STAT:
		{
			RefJavaAST tmp77_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EMPTY_STAT);
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::elist(RefJavaAST _t) {
	RefJavaAST elist_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t120 = _t;
		RefJavaAST tmp78_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ELIST);
		_t = _t->getFirstChild();
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==EXPR)) {
				expression(_t);
				_t = _retTree;
			}
			else {
				goto _loop122;
			}
			
		}
		_loop122:;
		}
		_t = __t120;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::caseGroup(RefJavaAST _t) {
	RefJavaAST caseGroup_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t107 = _t;
		RefJavaAST tmp79_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CASE_GROUP);
		_t = _t->getFirstChild();
		{
		int _cnt110=0;
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case LITERAL_case:
			{
				RefJavaAST __t109 = _t;
				RefJavaAST tmp80_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_case);
				_t = _t->getFirstChild();
				expression(_t);
				_t = _retTree;
				_t = __t109;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_default:
			{
				RefJavaAST tmp81_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_default);
				_t = _t->getNextSibling();
				break;
			}
			default:
			{
				if ( _cnt110>=1 ) { goto _loop110; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));}
			}
			}
			_cnt110++;
		}
		_loop110:;
		}
		slist(_t);
		_t = _retTree;
		_t = __t107;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::tryBlock(RefJavaAST _t) {
	RefJavaAST tryBlock_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t112 = _t;
		RefJavaAST tmp82_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_try);
		_t = _t->getFirstChild();
		slist(_t);
		_t = _retTree;
		{
		for (;;) {
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			if ((_t->getType()==LITERAL_catch)) {
				handler(_t);
				_t = _retTree;
			}
			else {
				goto _loop114;
			}
			
		}
		_loop114:;
		}
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case LITERAL_finally:
		{
			RefJavaAST __t116 = _t;
			RefJavaAST tmp83_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_finally);
			_t = _t->getFirstChild();
			slist(_t);
			_t = _retTree;
			_t = __t116;
			_t = _t->getNextSibling();
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		_t = __t112;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::handler(RefJavaAST _t) {
	RefJavaAST handler_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t118 = _t;
		RefJavaAST tmp84_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_catch);
		_t = _t->getFirstChild();
		parameterDef(_t);
		_t = _retTree;
		slist(_t);
		_t = _retTree;
		_t = __t118;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::expr(RefJavaAST _t) {
	RefJavaAST expr_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case QUESTION:
		{
			RefJavaAST __t126 = _t;
			RefJavaAST tmp85_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),QUESTION);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t126;
			_t = _t->getNextSibling();
			break;
		}
		case ASSIGN:
		{
			RefJavaAST __t127 = _t;
			RefJavaAST tmp86_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t127;
			_t = _t->getNextSibling();
			break;
		}
		case PLUS_ASSIGN:
		{
			RefJavaAST __t128 = _t;
			RefJavaAST tmp87_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PLUS_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t128;
			_t = _t->getNextSibling();
			break;
		}
		case MINUS_ASSIGN:
		{
			RefJavaAST __t129 = _t;
			RefJavaAST tmp88_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),MINUS_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t129;
			_t = _t->getNextSibling();
			break;
		}
		case STAR_ASSIGN:
		{
			RefJavaAST __t130 = _t;
			RefJavaAST tmp89_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STAR_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t130;
			_t = _t->getNextSibling();
			break;
		}
		case DIV_ASSIGN:
		{
			RefJavaAST __t131 = _t;
			RefJavaAST tmp90_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DIV_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t131;
			_t = _t->getNextSibling();
			break;
		}
		case MOD_ASSIGN:
		{
			RefJavaAST __t132 = _t;
			RefJavaAST tmp91_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),MOD_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t132;
			_t = _t->getNextSibling();
			break;
		}
		case SR_ASSIGN:
		{
			RefJavaAST __t133 = _t;
			RefJavaAST tmp92_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SR_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t133;
			_t = _t->getNextSibling();
			break;
		}
		case BSR_ASSIGN:
		{
			RefJavaAST __t134 = _t;
			RefJavaAST tmp93_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BSR_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t134;
			_t = _t->getNextSibling();
			break;
		}
		case SL_ASSIGN:
		{
			RefJavaAST __t135 = _t;
			RefJavaAST tmp94_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SL_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t135;
			_t = _t->getNextSibling();
			break;
		}
		case BAND_ASSIGN:
		{
			RefJavaAST __t136 = _t;
			RefJavaAST tmp95_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BAND_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t136;
			_t = _t->getNextSibling();
			break;
		}
		case BXOR_ASSIGN:
		{
			RefJavaAST __t137 = _t;
			RefJavaAST tmp96_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BXOR_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t137;
			_t = _t->getNextSibling();
			break;
		}
		case BOR_ASSIGN:
		{
			RefJavaAST __t138 = _t;
			RefJavaAST tmp97_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BOR_ASSIGN);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t138;
			_t = _t->getNextSibling();
			break;
		}
		case LOR:
		{
			RefJavaAST __t139 = _t;
			RefJavaAST tmp98_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LOR);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t139;
			_t = _t->getNextSibling();
			break;
		}
		case LAND:
		{
			RefJavaAST __t140 = _t;
			RefJavaAST tmp99_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LAND);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t140;
			_t = _t->getNextSibling();
			break;
		}
		case BOR:
		{
			RefJavaAST __t141 = _t;
			RefJavaAST tmp100_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BOR);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t141;
			_t = _t->getNextSibling();
			break;
		}
		case BXOR:
		{
			RefJavaAST __t142 = _t;
			RefJavaAST tmp101_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BXOR);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t142;
			_t = _t->getNextSibling();
			break;
		}
		case BAND:
		{
			RefJavaAST __t143 = _t;
			RefJavaAST tmp102_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BAND);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t143;
			_t = _t->getNextSibling();
			break;
		}
		case NOT_EQUAL:
		{
			RefJavaAST __t144 = _t;
			RefJavaAST tmp103_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),NOT_EQUAL);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t144;
			_t = _t->getNextSibling();
			break;
		}
		case EQUAL:
		{
			RefJavaAST __t145 = _t;
			RefJavaAST tmp104_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),EQUAL);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t145;
			_t = _t->getNextSibling();
			break;
		}
		case LT_:
		{
			RefJavaAST __t146 = _t;
			RefJavaAST tmp105_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LT_);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t146;
			_t = _t->getNextSibling();
			break;
		}
		case GT:
		{
			RefJavaAST __t147 = _t;
			RefJavaAST tmp106_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),GT);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t147;
			_t = _t->getNextSibling();
			break;
		}
		case LE:
		{
			RefJavaAST __t148 = _t;
			RefJavaAST tmp107_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LE);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t148;
			_t = _t->getNextSibling();
			break;
		}
		case GE:
		{
			RefJavaAST __t149 = _t;
			RefJavaAST tmp108_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),GE);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t149;
			_t = _t->getNextSibling();
			break;
		}
		case SL:
		{
			RefJavaAST __t150 = _t;
			RefJavaAST tmp109_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SL);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t150;
			_t = _t->getNextSibling();
			break;
		}
		case SR:
		{
			RefJavaAST __t151 = _t;
			RefJavaAST tmp110_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),SR);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t151;
			_t = _t->getNextSibling();
			break;
		}
		case BSR:
		{
			RefJavaAST __t152 = _t;
			RefJavaAST tmp111_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BSR);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t152;
			_t = _t->getNextSibling();
			break;
		}
		case PLUS:
		{
			RefJavaAST __t153 = _t;
			RefJavaAST tmp112_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),PLUS);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t153;
			_t = _t->getNextSibling();
			break;
		}
		case MINUS:
		{
			RefJavaAST __t154 = _t;
			RefJavaAST tmp113_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),MINUS);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t154;
			_t = _t->getNextSibling();
			break;
		}
		case DIV:
		{
			RefJavaAST __t155 = _t;
			RefJavaAST tmp114_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DIV);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t155;
			_t = _t->getNextSibling();
			break;
		}
		case MOD:
		{
			RefJavaAST __t156 = _t;
			RefJavaAST tmp115_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),MOD);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t156;
			_t = _t->getNextSibling();
			break;
		}
		case STAR:
		{
			RefJavaAST __t157 = _t;
			RefJavaAST tmp116_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STAR);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t157;
			_t = _t->getNextSibling();
			break;
		}
		case INC:
		{
			RefJavaAST __t158 = _t;
			RefJavaAST tmp117_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),INC);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t158;
			_t = _t->getNextSibling();
			break;
		}
		case DEC:
		{
			RefJavaAST __t159 = _t;
			RefJavaAST tmp118_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DEC);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t159;
			_t = _t->getNextSibling();
			break;
		}
		case POST_INC:
		{
			RefJavaAST __t160 = _t;
			RefJavaAST tmp119_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),POST_INC);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t160;
			_t = _t->getNextSibling();
			break;
		}
		case POST_DEC:
		{
			RefJavaAST __t161 = _t;
			RefJavaAST tmp120_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),POST_DEC);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t161;
			_t = _t->getNextSibling();
			break;
		}
		case BNOT:
		{
			RefJavaAST __t162 = _t;
			RefJavaAST tmp121_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),BNOT);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t162;
			_t = _t->getNextSibling();
			break;
		}
		case LNOT:
		{
			RefJavaAST __t163 = _t;
			RefJavaAST tmp122_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LNOT);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t163;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_instanceof:
		{
			RefJavaAST __t164 = _t;
			RefJavaAST tmp123_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_instanceof);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t164;
			_t = _t->getNextSibling();
			break;
		}
		case UNARY_MINUS:
		{
			RefJavaAST __t165 = _t;
			RefJavaAST tmp124_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),UNARY_MINUS);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t165;
			_t = _t->getNextSibling();
			break;
		}
		case UNARY_PLUS:
		{
			RefJavaAST __t166 = _t;
			RefJavaAST tmp125_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),UNARY_PLUS);
			_t = _t->getFirstChild();
			expr(_t);
			_t = _retTree;
			_t = __t166;
			_t = _t->getNextSibling();
			break;
		}
		case TYPE:
		case TYPECAST:
		case INDEX_OP:
		case METHOD_CALL:
		case IDENT:
		case DOT:
		case LITERAL_this:
		case LITERAL_super:
		case LITERAL_true:
		case LITERAL_false:
		case LITERAL_null:
		case LITERAL_new:
		case NUM_INT:
		case CHAR_LITERAL:
		case STRING_LITERAL:
		case NUM_FLOAT:
		{
			primaryExpression(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::primaryExpression(RefJavaAST _t) {
	RefJavaAST primaryExpression_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case IDENT:
		{
			RefJavaAST tmp126_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
			_t = _t->getNextSibling();
			break;
		}
		case DOT:
		{
			RefJavaAST __t168 = _t;
			RefJavaAST tmp127_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),DOT);
			_t = _t->getFirstChild();
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case TYPE:
			case TYPECAST:
			case INDEX_OP:
			case POST_INC:
			case POST_DEC:
			case METHOD_CALL:
			case UNARY_MINUS:
			case UNARY_PLUS:
			case IDENT:
			case DOT:
			case STAR:
			case ASSIGN:
			case PLUS_ASSIGN:
			case MINUS_ASSIGN:
			case STAR_ASSIGN:
			case DIV_ASSIGN:
			case MOD_ASSIGN:
			case SR_ASSIGN:
			case BSR_ASSIGN:
			case SL_ASSIGN:
			case BAND_ASSIGN:
			case BXOR_ASSIGN:
			case BOR_ASSIGN:
			case QUESTION:
			case LOR:
			case LAND:
			case BOR:
			case BXOR:
			case BAND:
			case NOT_EQUAL:
			case EQUAL:
			case LT_:
			case GT:
			case LE:
			case GE:
			case LITERAL_instanceof:
			case SL:
			case SR:
			case BSR:
			case PLUS:
			case MINUS:
			case DIV:
			case MOD:
			case INC:
			case DEC:
			case BNOT:
			case LNOT:
			case LITERAL_this:
			case LITERAL_super:
			case LITERAL_true:
			case LITERAL_false:
			case LITERAL_null:
			case LITERAL_new:
			case NUM_INT:
			case CHAR_LITERAL:
			case STRING_LITERAL:
			case NUM_FLOAT:
			{
				expr(_t);
				_t = _retTree;
				{
				if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				switch ( _t->getType()) {
				case IDENT:
				{
					RefJavaAST tmp128_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
					_t = _t->getNextSibling();
					break;
				}
				case INDEX_OP:
				{
					arrayIndex(_t);
					_t = _retTree;
					break;
				}
				case LITERAL_this:
				{
					RefJavaAST tmp129_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_this);
					_t = _t->getNextSibling();
					break;
				}
				case LITERAL_class:
				{
					RefJavaAST tmp130_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_class);
					_t = _t->getNextSibling();
					break;
				}
				case LITERAL_new:
				{
					RefJavaAST __t171 = _t;
					RefJavaAST tmp131_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_new);
					_t = _t->getFirstChild();
					RefJavaAST tmp132_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),IDENT);
					_t = _t->getNextSibling();
					elist(_t);
					_t = _retTree;
					_t = __t171;
					_t = _t->getNextSibling();
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
				}
				}
				}
				break;
			}
			case ARRAY_DECLARATOR:
			{
				RefJavaAST __t172 = _t;
				RefJavaAST tmp133_AST_in = _t;
				match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ARRAY_DECLARATOR);
				_t = _t->getFirstChild();
				type(_t);
				_t = _retTree;
				_t = __t172;
				_t = _t->getNextSibling();
				break;
			}
			case LITERAL_void:
			case LITERAL_boolean:
			case LITERAL_byte:
			case LITERAL_char:
			case LITERAL_short:
			case LITERAL_int:
			case LITERAL_float:
			case LITERAL_long:
			case LITERAL_double:
			{
				builtInType(_t);
				_t = _retTree;
				{
				if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
					_t = ASTNULL;
				switch ( _t->getType()) {
				case LITERAL_class:
				{
					RefJavaAST tmp134_AST_in = _t;
					match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_class);
					_t = _t->getNextSibling();
					break;
				}
				case 3:
				{
					break;
				}
				default:
				{
					throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
				}
				}
				}
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			_t = __t168;
			_t = _t->getNextSibling();
			break;
		}
		case INDEX_OP:
		{
			arrayIndex(_t);
			_t = _retTree;
			break;
		}
		case METHOD_CALL:
		{
			RefJavaAST __t174 = _t;
			RefJavaAST tmp135_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),METHOD_CALL);
			_t = _t->getFirstChild();
			primaryExpression(_t);
			_t = _retTree;
			elist(_t);
			_t = _retTree;
			_t = __t174;
			_t = _t->getNextSibling();
			break;
		}
		case TYPECAST:
		{
			RefJavaAST __t175 = _t;
			RefJavaAST tmp136_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),TYPECAST);
			_t = _t->getFirstChild();
			typeSpec(_t);
			_t = _retTree;
			expr(_t);
			_t = _retTree;
			_t = __t175;
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_new:
		{
			newExpression(_t);
			_t = _retTree;
			break;
		}
		case NUM_INT:
		case CHAR_LITERAL:
		case STRING_LITERAL:
		case NUM_FLOAT:
		{
			constant(_t);
			_t = _retTree;
			break;
		}
		case LITERAL_super:
		{
			RefJavaAST tmp137_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_super);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_true:
		{
			RefJavaAST tmp138_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_true);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_false:
		{
			RefJavaAST tmp139_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_false);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_this:
		{
			RefJavaAST tmp140_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_this);
			_t = _t->getNextSibling();
			break;
		}
		case LITERAL_null:
		{
			RefJavaAST tmp141_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_null);
			_t = _t->getNextSibling();
			break;
		}
		case TYPE:
		{
			typeSpec(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::arrayIndex(RefJavaAST _t) {
	RefJavaAST arrayIndex_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t177 = _t;
		RefJavaAST tmp142_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),INDEX_OP);
		_t = _t->getFirstChild();
		primaryExpression(_t);
		_t = _retTree;
		expression(_t);
		_t = _retTree;
		_t = __t177;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::newExpression(RefJavaAST _t) {
	RefJavaAST newExpression_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t180 = _t;
		RefJavaAST tmp143_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),LITERAL_new);
		_t = _t->getFirstChild();
		type(_t);
		_t = _retTree;
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ARRAY_DECLARATOR:
		{
			newArrayDeclarator(_t);
			_t = _retTree;
			{
			if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
				_t = ASTNULL;
			switch ( _t->getType()) {
			case ARRAY_INIT:
			{
				arrayInitializer(_t);
				_t = _retTree;
				break;
			}
			case 3:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
			}
			}
			}
			break;
		}
		case ELIST:
		{
			elist(_t);
			_t = _retTree;
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		_t = __t180;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::constant(RefJavaAST _t) {
	RefJavaAST constant_AST_in = _t;
	
	try {      // for error handling
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case NUM_INT:
		{
			RefJavaAST tmp144_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),NUM_INT);
			_t = _t->getNextSibling();
			break;
		}
		case CHAR_LITERAL:
		{
			RefJavaAST tmp145_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),CHAR_LITERAL);
			_t = _t->getNextSibling();
			break;
		}
		case STRING_LITERAL:
		{
			RefJavaAST tmp146_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),STRING_LITERAL);
			_t = _t->getNextSibling();
			break;
		}
		case NUM_FLOAT:
		{
			RefJavaAST tmp147_AST_in = _t;
			match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),NUM_FLOAT);
			_t = _t->getNextSibling();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

void JavaStoreWalker::newArrayDeclarator(RefJavaAST _t) {
	RefJavaAST newArrayDeclarator_AST_in = _t;
	
	try {      // for error handling
		RefJavaAST __t184 = _t;
		RefJavaAST tmp148_AST_in = _t;
		match(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t),ARRAY_DECLARATOR);
		_t = _t->getFirstChild();
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case ARRAY_DECLARATOR:
		{
			newArrayDeclarator(_t);
			_t = _retTree;
			break;
		}
		case 3:
		case EXPR:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		{
		if (_t == static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = ASTNULL;
		switch ( _t->getType()) {
		case EXPR:
		{
			expression(_t);
			_t = _retTree;
			break;
		}
		case 3:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(static_cast<ANTLR_USE_NAMESPACE(antlr)RefAST>(_t));
		}
		}
		}
		_t = __t184;
		_t = _t->getNextSibling();
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		reportError(ex);
		if ( _t != static_cast<RefJavaAST>(ANTLR_USE_NAMESPACE(antlr)nullAST) )
			_t = _t->getNextSibling();
	}
	_retTree = _t;
}

RefJavaAST JavaStoreWalker::getAST()
{
	return returnAST;
}

const char* JavaStoreWalker::_tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"BLOCK",
	"MODIFIERS",
	"OBJBLOCK",
	"SLIST",
	"CTOR_DEF",
	"METHOD_DEF",
	"VARIABLE_DEF",
	"INSTANCE_INIT",
	"STATIC_INIT",
	"TYPE",
	"CLASS_DEF",
	"INTERFACE_DEF",
	"PACKAGE_DEF",
	"ARRAY_DECLARATOR",
	"EXTENDS_CLAUSE",
	"IMPLEMENTS_CLAUSE",
	"PARAMETERS",
	"PARAMETER_DEF",
	"LABELED_STAT",
	"TYPECAST",
	"INDEX_OP",
	"POST_INC",
	"POST_DEC",
	"METHOD_CALL",
	"EXPR",
	"ARRAY_INIT",
	"IMPORT",
	"UNARY_MINUS",
	"UNARY_PLUS",
	"CASE_GROUP",
	"ELIST",
	"FOR_INIT",
	"FOR_CONDITION",
	"FOR_ITERATOR",
	"EMPTY_STAT",
	"\"final\"",
	"\"abstract\"",
	"\"package\"",
	"SEMI",
	"\"import\"",
	"LBRACK",
	"RBRACK",
	"\"void\"",
	"\"boolean\"",
	"\"byte\"",
	"\"char\"",
	"\"short\"",
	"\"int\"",
	"\"float\"",
	"\"long\"",
	"\"double\"",
	"IDENT",
	"DOT",
	"STAR",
	"\"private\"",
	"\"public\"",
	"\"protected\"",
	"\"static\"",
	"\"transient\"",
	"\"native\"",
	"\"threadsafe\"",
	"\"synchronized\"",
	"\"volatile\"",
	"\"class\"",
	"\"extends\"",
	"\"interface\"",
	"LCURLY",
	"RCURLY",
	"COMMA",
	"\"implements\"",
	"LPAREN",
	"RPAREN",
	"ASSIGN",
	"\"throws\"",
	"COLON",
	"\"if\"",
	"\"else\"",
	"\"for\"",
	"\"while\"",
	"\"do\"",
	"\"break\"",
	"\"continue\"",
	"\"return\"",
	"\"switch\"",
	"\"throw\"",
	"\"case\"",
	"\"default\"",
	"\"try\"",
	"\"finally\"",
	"\"catch\"",
	"PLUS_ASSIGN",
	"MINUS_ASSIGN",
	"STAR_ASSIGN",
	"DIV_ASSIGN",
	"MOD_ASSIGN",
	"SR_ASSIGN",
	"BSR_ASSIGN",
	"SL_ASSIGN",
	"BAND_ASSIGN",
	"BXOR_ASSIGN",
	"BOR_ASSIGN",
	"QUESTION",
	"LOR",
	"LAND",
	"BOR",
	"BXOR",
	"BAND",
	"NOT_EQUAL",
	"EQUAL",
	"LT_",
	"GT",
	"LE",
	"GE",
	"\"instanceof\"",
	"SL",
	"SR",
	"BSR",
	"PLUS",
	"MINUS",
	"DIV",
	"MOD",
	"INC",
	"DEC",
	"BNOT",
	"LNOT",
	"\"this\"",
	"\"super\"",
	"\"true\"",
	"\"false\"",
	"\"null\"",
	"\"new\"",
	"NUM_INT",
	"CHAR_LITERAL",
	"STRING_LITERAL",
	"NUM_FLOAT",
	"WS_",
	"SL_COMMENT",
	"ML_COMMENT",
	"ESC",
	"HEX_DIGIT",
	"VOCAB",
	"EXPONENT",
	"FLOAT_SUFFIX",
	"\"const\"",
	0
};

const unsigned long JavaStoreWalker::_tokenSet_0_data_[] = { 0UL, 4227858816UL, 7UL, 0UL, 524288UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// "final" "abstract" "private" "public" "protected" "static" "transient" 
// "native" "threadsafe" "synchronized" "volatile" "const" 
const ANTLR_USE_NAMESPACE(antlr)BitSet JavaStoreWalker::_tokenSet_0(_tokenSet_0_data_,12);
const unsigned long JavaStoreWalker::_tokenSet_1_data_[] = { 272680064UL, 64UL, 167673858UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// SLIST VARIABLE_DEF CLASS_DEF INTERFACE_DEF LABELED_STAT EXPR EMPTY_STAT 
// "synchronized" "if" "for" "while" "do" "break" "continue" "return" "switch" 
// "throw" "try" 
const ANTLR_USE_NAMESPACE(antlr)BitSet JavaStoreWalker::_tokenSet_1(_tokenSet_1_data_,8);


