/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package at.opendrone.opendrone;

import java.util.Iterator;

/**
 *
 * @author Mike
 */
public class NodeIterator<E> implements Iterator<E> {

    private Node head;

    public NodeIterator(Node head) {
        this.head = head;
    }

    @Override
    public boolean hasNext() {
        if (head != null) {
            return true;
        }
        return false;
    }

    @Override
    public E next() {
        Node ret = head;
        head = head.next;
        try{
            return (E) ret.val;
        }catch(Exception e){
            return null;
        }

    }

}
