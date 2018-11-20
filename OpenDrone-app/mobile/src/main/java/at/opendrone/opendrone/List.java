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
import java.util.ListIterator;

/**
 *
 * @author 20150032
 */
public class List<T> implements java.util.List<T> {
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

    @Override
    public T set(int index, T element) {
        return null;
    }

    @Override
    public void add(int index, T element) {

    }

    public int size(){
        return size;
    }

    @Override
    public boolean isEmpty() {
        return false;
    }

    @Override
    public boolean contains(@NonNull Object o) {
        return false;
    }

    @NonNull
    @Override
    public Iterator<T> iterator() {
        return null;
    }

    @NonNull
    @Override
    public Object[] toArray() {
        return new Object[0];
    }

    @Override
    public <T1> T1[] toArray(@Nullable T1[] a) {
        return null;
    }

    @Override
    public boolean add(T t) {
        append(t);
        return true;
    }

    @Override
    public boolean remove(@NonNull Object o) {
        removeFromList(o);
        return true;
    }

    @Override
    public boolean containsAll(@NonNull Collection<?> c) {
        return false;
    }

    @Override
    public boolean addAll(@NonNull Collection<? extends T> c) {
        return false;
    }

    @Override
    public boolean addAll(int index, @NonNull Collection<? extends T> c) {
        return false;
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
        clear();
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

    @Override
    public int indexOf(@NonNull Object o) {
        return 0;
    }

    @Override
    public int lastIndexOf(@NonNull Object o) {
        return 0;
    }

    @NonNull
    @Override
    public ListIterator<T> listIterator() {
        return null;
    }


    @NonNull
    @Override
    public ListIterator<T> listIterator(int index) {
        return null;
    }

    @NonNull
    @Override
    public java.util.List<T> subList(int fromIndex, int toIndex) {
        return null;
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
    
   /* public void invert(){
        Node curT;
        Node curH;
        Node cur;
        
        cur=tail;
        tail=head;
        head=cur;
        curT=tail;
        curH=head;
        while(curT!=curH){
            curT.prev=curH;
            curT=curT.prev;
            curH=curH.next;
        }
        
    }*/
    public void print(){
        Node cur = head;
        while(cur != null){
            System.out.print(cur.val+" ");
            cur=cur.next;
        }
        System.out.println();
    }
    
   
    
    
    
}
