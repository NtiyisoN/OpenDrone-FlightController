/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package at.opendrone.opendrone;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.ListIterator;

/**
 *
 * @author 20150032
 */
public class List<T> implements java.util.List<Node<T>> {
    Node head, tail;
    int size;

    public List() {
        head=null;
        tail=null;
    }
    
    public void append(T val){
        Node p = new Node<T>(val);
        if(head==null){
            head = p;
        }else{
            p.prev=tail;
            tail.next=p;
        }
        tail=p;
        size++;
    }
    
    public Node<T> getNode(int i){
        int cnt = 0;
        Node cur = head;
        if(i==0){
            return cur;
        }
        while(cur!=null && cnt!=i){
            if(cnt==i){
                return cur;
            }cnt++;
            cur=cur.next;
        }
        return null;
    }

    public void prepend(T val){
        Node p = new Node<T>(val);
        
        if(head==null){
            head=p;
        }else{
            head.prev=head;
            p.next=head;
        }
        head=p;
        size++;
    }
    
    public boolean delete(int val){
        Node cur  = head;
        Node prev = null;
        while(cur!=null&&!cur.val.equals(val)){
            prev=cur;
            cur=cur.next;
        }
        if(cur==null){
            return false;
        }else{
            if(cur==head){
                head=cur.next;
            }else if(cur==tail){
                tail=cur.prev;
                tail.next=null;
            }else{
                prev.next=cur.next;
            }
        }
        size--;
        return true;
    }
    
    public boolean removeNode(int i){
        Node cur = head;
        Node prev = null;
        int cnt = 0;
        while(cur!=null && cnt != i){
            cur = cur.next;
        }
        if(cur==null){
            return false;
        }else{
            if(cur==head){
                head=cur.next;
            }else if(cur==tail){
                tail=cur.prev;
                tail.next=null;
            }else{
                prev.next=cur.next;
            }
        }
        size--;
        return true;
    }

    public void appendList (List l){
        tail.next=l.head;
        tail=l.tail;
        size+=l.size();
    }
    
    public void prependList (List l){
        l.tail.next=head;
        head=l.head;
        size+=l.size();
    }
    
    public void print(){
        Node cur = head;
        while(cur != null){
            System.out.print(cur.val+" ");
            cur=cur.next;
        }
        System.out.println();
    }


    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return !(size > 0);
    }

    @Override
    public boolean contains(@Nullable Object o) {
        Node cur = head;
        Node toCompare = new Node(o);
        int pos = 0;
        boolean contains = false;

        while(cur != null){
            if(cur.val == toCompare.val){
                contains = true;
            }
            cur = cur.next;
        }
        return contains;
    }

    @NonNull
    @Override
    public Iterator<Node<T>> iterator() {
        return null;
    }

    @Nullable
    @Override
    public Object[] toArray() {
        Object[] arr = new Object[size];
        Node cur = head;
        int pos = 0;

        while(cur != null){
            arr[pos] = cur.val;
            pos++;
            cur = cur.next;
        }


        return arr;
    }

    @Override
    public <T> T[] toArray(@Nullable T[] a) {
        return null;
    }

    @Override
    public boolean containsAll(@Nullable Collection<?> c) {
        return false;
    }

    @Override
    public boolean add(Node<T> tNode) {
        return false;
    }

    @Override
    public void add(int index, Node<T> element) {
        Node p = element;
        Node cur = head;
        int pos = 0;
        if(head==null){
            head = p;
        }else{
            p.prev=tail;
            tail.next=p;
        }

        while(cur != null){
            if(pos == index){
                Node next = cur.next;
                cur.next = p;
                p.next = next;
            }
            cur = cur.next;
        }
        tail = cur;

        size++;
    }

    @Override
    public boolean addAll(@Nullable Collection<? extends Node<T>> c) {
        return false;
    }

    @Override
    public boolean addAll(int index, @NonNull Collection<? extends Node<T>> c) {
        return false;
    }

    @Override
    public boolean remove(@Nullable Object o) {
        return false;
    }

    @Override
    public Node<T> remove(int index) {
        Node cur = head;
        Node prev = null;
        int cnt = 0;
        while(cur!=null && cnt != index){
            cur = cur.next;
        }
        if(cur == null){
            return null;
        }else{
            if(cur==head){
                head=cur.next;
            }else if(cur==tail){
                tail=cur.prev;
                tail.next=null;
            }else{
                prev.next=cur.next;
            }
        }
        size--;
        return cur;
    }

    @Override
    public boolean removeAll(@NonNull Collection<?> c) {
        return false;
    }

    @Override
    public boolean retainAll(@NonNull Collection<?> c) {
        return false;
    }

    @Override
    public void clear() {
        head = new Node(null);
    }

    @Override
    public Node<T> get(int index) {
        Node cur = head;
        Node get = null;
        int pos = 0;

        while(cur != null){
            if(pos == index){
                get = cur;
            }
            cur = cur.next;
        }
        return get;
    }

    @Override
    public Node<T> set(int index, Node<T> element) {
        Node p = element;
        Node cur = head;
        Node next;
        int pos = 0;

        while(cur != null){
            if(pos == index){
                next = cur.next;
                cur = element;
                cur.next = next;
            }
            cur = cur.next;
        }
        return element;
    }

    @Override
    public int indexOf(@Nullable Object o) {
        int index = 0;
        Node cur = head;
        Node toCompare = new Node(o);

        while(cur!=null){
            if(cur.val == toCompare.val){
                break;
            }
            index++;
            cur=cur.next;
        }
        return index;
    }

    @Override
    public int lastIndexOf(@Nullable Object o) {
        //Method returns -1 when no position was found;
        int lastIndex = -1;
        int index = 0;
        Node cur = head;
        Node toCompare = new Node(o);

        while(cur!=null){
            if(cur.val == toCompare.val){
                lastIndex = index;
            }
            index++;
            cur=cur.next;
        }

        return lastIndex;
    }

    //No Iterators implemented
    @NonNull
    @Override
    public ListIterator<Node<T>> listIterator() {
        return null;
    }

    @NonNull
    @Override
    public ListIterator<Node<T>> listIterator(int index) {
        return null;
    }

    @NonNull
    @Override
    public java.util.List<Node<T>> subList(int fromIndex, int toIndex) {
        int count = 0;
        Node cur = head;
        java.util.List<Node<T>> sub = new LinkedList<>();

        if(fromIndex > size || toIndex > size){
            return null;
        }

        while(cur!=null){
            if(count >= fromIndex && count <= toIndex){
                sub.add(cur);
            }
            count++;
            cur=cur.next;
        }
        return sub;
    }
}
