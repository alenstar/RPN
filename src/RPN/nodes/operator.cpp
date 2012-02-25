/******************************************************************************
   Copyright 2012 Kevin Burk
   
   This file is part of RPN.
  
   RPN is free software: you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.
  
   RPN is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.
  
   You should have received a copy of the GNU General Public License along
   with RPN.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#ifndef RPN_LIBHEADER
	#include <sstream>
#endif

#include "operator.h"
#include "../exception.h"
#include "../parsers/infix.h"

namespace RPN
{
	OperatorNode::OperatorNode(int precedence, Associativity associativity, int arguments):
		mArguments(arguments),
		mAssociativity(associativity),
		mPrecedence(precedence)
	{
		if((unsigned int) mArguments > 2)
		{
			std::ostringstream mess;
			mess << "Invalid argument count: Expected 1 or 2; got " << mArguments;
			throw Exception(mess.str());
		}
	}
	
	int OperatorNode::arguments() const
	{
		return mArguments;
	}
	
	int OperatorNode::associativity() const
	{
		return mAssociativity;
	}
	
	void OperatorNode::infixParse(InfixParser& parser, Parser::Token& token) const
	{
		int comp = precedence() - isRightAssociative();
		
		while(parser.hasStack())
		{
			Parser::Token token = parser.top();
			const Node* node = token.node;
			
			if(node->type() == Node::OPERATOR && ((const OperatorNode*)node)->precedence() >= comp)
			{
				parser.shunt();
			}
			else
			{
				break;
			}
		}
		
		parser.push_to_stack(token);
	}
	
	Node::Type OperatorNode::infixPresents() const
	{
		if(mArguments == 2 || isRightAssociative())
		{
			return Node::OPERATOR;
		}
		else
		{
			return Node::VALUE;
		}
	}
	
	Node::Type OperatorNode::infixSucceeds() const
	{
		if(mArguments == 2 || isLeftAssociative())
		{
			return Node::VALUE;
		}
		else
		{
			return Node::OPERATOR;
		}
	}
	
	bool OperatorNode::isLeftAssociative() const
	{
		return (mAssociativity != RIGHT);
	}
	
	bool OperatorNode::isRightAssociative() const
	{
		return (mAssociativity != LEFT);
	}
	
	int OperatorNode::precedence() const
	{
		return mPrecedence;
	}
	
	Node::Type OperatorNode::type() const
	{
		return Node::OPERATOR;
	}
}
