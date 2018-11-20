/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package at.opendrone.opendrone;

/**
 *
 * @author 20150032
 */
public class Node<T> {
    T val;
    Node next;
    Node prev;

    public Node(T val) {
        this.val = val;
        next=null;
        prev=null;
    }

    @Override
    public String toString() {
        if(this!=null)
            return "("+val+")";
        return "()";
    }
    
    
}
